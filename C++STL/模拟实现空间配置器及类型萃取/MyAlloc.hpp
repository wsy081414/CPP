
#pragma once
#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include<cstdarg>
#include<cstdlib>

using namespace std;



// Trace 跟踪

#define __DEBUG__

FILE* fOut = fopen("trace.log", "w");

static string GetFileName(const string& path)
{
	char ch = '/';

#ifdef _WIN32
	ch = '\\';
#endif

	size_t pos = path.rfind(ch);
	if (pos == string::npos)
		return path;
	else
		return path.substr(pos + 1);
}
// 用于调试追溯的trace log
inline static void __trace_debug(const char* function,
	const char * filename, int line, char* format, ...)
{
	// 读取配置文件
#ifdef __DEBUG__

	// 输出调用函数的信息
	fprintf(stdout, "【 %s:%d】%s", GetFileName(filename).c_str(), line, function);
	fprintf(fOut, "【 %s:%d】%s", GetFileName(filename).c_str(), line, function);

	// 输出用户打的trace信息
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	vfprintf(fOut, format, args);
	fprintf(fOut, "%c", '\n');
	fprintf(stdout, "%c", '\n');
	va_end(args);
#endif
}

#define __TRACE_DEBUG(...)  \
	__trace_debug(__FUNCTION__, __FILE__, __LINE__, __VA_ARGS__);







//内存分配的第一级配置器，在第一级当中，进行判断调用第一级分配器还是第二级分配器，如果是第二级如何操作。
template<int inst>
class MallocAllocTemplate
{
	//定义一个函数指针类型
	typedef void(*_FuncP)();
private:
	static _FuncP MallocAllocOOMHandler;	//处理内存不够的一个句柄
	static void *OOMMalloc(size_t);		//处理内存不够的函数
public:
	static void *Allocate(size_t n)
	{

		__TRACE_DEBUG("向一级空间配置器申请了%u个字节", n);
		void *result = malloc(n);	//进行分配n个内存空间
		if (result == NULL)			//判断是否分配成功
			result = OOMMalloc(n);	//内存分配不成功，进入OOMMalloc查看是否可以操作系统释放内存分配，如果无法分配，抛出异常。
		return result;
	}
	static void DeAllocate(void *p, size_t n)
	{
		__TRACE_DEBUG("一级空间配置器释放了%u个字节", n);
		free(p);		//第一级空间配置器直接使用free
	}

	static _FuncP SetMallocHandler(_FuncP f)		//设置文件句柄，通过这个函数可以使知道操作系统中是否有还可以分配的内存。
	{
		_FuncP old = MallocAllocOOMHandler;			//
		MallocAllocOOMHandler = f;					//设置文件句柄
		return old;									//返回旧的，操作系统惯用写法。
	}
};


template<int inst>
typename MallocAllocTemplate<inst>::
_FuncP MallocAllocTemplate<inst>::
MallocAllocOOMHandler = 0;	//初始化指针为空

template<int inst>
void * MallocAllocTemplate<inst>::OOMMalloc(size_t n)		//内存不够的函数
{
	_FuncP MyMallocHandler;
	void * result;
	while (1)
	{
		MyMallocHandler = MallocAllocOOMHandler;		//
		if (MyMallocHandler == 0)						//看文件句柄是否为空，为空就抛出异常
			throw bad_alloc();							//不为空，说明操作系统中还有可以内存可以通过释放再来进行分配。

		//在这里进行的就是set_new_handler机制，可以使得更多的空间变成可分配的空间。
		//这里可能就会分配出内存，达到最佳性能。
		(*MyMallocHandler)();							//释放一些无关内存，用来分配


		result = malloc(n);
		if (result)										//如果分配成功，那么就返回
			return result;								//否则，继续进行循环查看
	}
}




//二级空间配置器的类
template<bool threads,int inst>
class DefaultAllocTemplate
{
private:
	enum { __ALIGN = 8 };								//最大的对齐数
	enum { __MAX_BYTES = 128 };							//最大字节数
	enum { __NFREELISTS = __MAX_BYTES / __ALIGN };		//自由链表的最大个数

	//得到一个bytes最近的一个8的整数倍整数
	static size_t ROUND_UP(size_t bytes) {
		return (((bytes)+__ALIGN - 1) & ~(__ALIGN - 1));
	}

	//计算bytes应该属于第几个自由链表。
	static  size_t FREELIST_INDEX(size_t bytes) {
		return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
	}
	//每一个人内存节点的结构
	union obj
	{
		union obj* FreeListLink;		//自由链表的指向下一个的指针，
		char client_data;				//提供给客户端查看
	};

	static obj* volatile FreeList[__NFREELISTS];		//自由链表的声明。每个自由链表中存放的是对象的指针类型。
	//填充空间配置器
	//此时的n都已经为8的整数倍，我们采用了ROUND_UP处理
	//最终这个函数所做就是返回n个对象大小的内存，然后填充自由链表
	static void*  Refill(size_t n /*n是对象的大小*/)
	{
		int nodjs = 20;		//默认从内存池中取得20个内存节点。



		char * chunk = ChunkAlloc(n, nodjs);		//在这里，进行取出内存块操作，查看最终取出的内存块是多大。

		obj *volatile *MyFreeList = NULL;

		obj *result = NULL;

		//当最少获得了一块内存块，直接返回给客户端                                                        
		if (nodjs == 1)		
		{
			return chunk;
		}
		//当获得了多块内存的，时候进行调整，把第一块返回给客户端，然后剩下的放到自由链表下面
	
		MyFreeList = FreeList + FREELIST_INDEX(n);		//找到在那个自由链表下放内存块。

		//注意，每次取也只是对象的大小的内存空间
		result = (obj*)chunk;

		obj* Cur=NULL;
		obj* Next = NULL;
		//把你所需要的大小的内存块从内存池当中取出来。
		*MyFreeList=Next = (obj *)(chunk + n);

		Cur = Next;
		//将所取出的内存块进行分配成为自由链表的形式
		size_t i = 1;

		while (1)
		{
			Next = (obj *)((char *)Next + n);	//取得下一块对象大小的内存块。

			//考虑最后一个节点的情况和其他节点
			if (nodjs - 1 == i)
			{
				Cur->FreeListLink = NULL;		//最后一个节点，给他放空。
				break;
			}
			else
				Cur->FreeListLink = Next;		//其他节点，都是该节点的下一个节点放Next。
			
			Cur = Next;		//更新Cur。

			i++;
		}

		return result;	//把最后第一块空间返回。
	}
	//用来从内存池中拿内存块，根据最终的nobjs的不同，返回开辟的情况的不同
	static char* ChunkAlloc(size_t size, int &nobjs)
	{
		char* result;
		size_t totalbytes = nobjs*size;
		size_t leftbytes = EndFree - StartFree;

		if (totalbytes <= leftbytes)
		{
			__TRACE_DEBUG("内存池中有足够%u个", nobjs);

			result = StartFree;
			StartFree += totalbytes;
			return result;
		}
		if (leftbytes >= size)
		{
			__TRACE_DEBUG("内存池中只能够%u个", nobjs);

			nobjs = leftbytes / size;
			result = StartFree;
			StartFree += nobjs*size;
			return result;
		}
		else
		{
			size_t bytestoget = 2 * totalbytes + ROUND_UP(HeapSize >> 4);
			__TRACE_DEBUG("内存池向系统请求%u个字节",bytestoget);

			if (leftbytes > 0)
			{
				__TRACE_DEBUG("内存池剩余%u个字节，放到自由链表%u下面", bytestoget, FREELIST_INDEX(leftbytes));

				obj*volatile* myfreelist = FreeList + FREELIST_INDEX(leftbytes);
				((obj*)StartFree)->FreeListLink = *myfreelist;
				*myfreelist =(obj*)StartFree;
			}

			StartFree = (char *)malloc(bytestoget);

			
			if (StartFree == NULL)
			{
				obj* volatile * myfreelist = NULL;
				for (size_t i = size; i < __MAX_BYTES; i++)
				{
					size_t index = FREELIST_INDEX(i);
					myfreelist = FreeList+index;
					obj* p = *myfreelist;
					if (p != NULL)
					{
						StartFree = (char *)p;
						EndFree = StartFree + i;
						*myfreelist = p->FreeListLink;
						return ChunkAlloc(size, nobjs);
					}
					EndFree = NULL;
					StartFree = (char *)MallocAllocTemplate<inst>::Allocate(bytestoget);
				}
			}
			HeapSize += bytestoget;
			EndFree = StartFree + bytestoget;
			return ChunkAlloc(size, nobjs);
		
		}
	
	}

public:
	static void* Allocate(size_t n)
	{
		obj* volatile*MyFreeList;	//指向一个自由链表节点
		obj* result;
		//当大于128个字节的时候，去调用第一级分配器
		if (n > (size_t)__MAX_BYTES)
			return MallocAllocTemplate<inst>::Allocate(n);

		__TRACE_DEBUG("向二级空间配置器申请了%u个字节", n);
		__TRACE_DEBUG("内存池剩余了%u个字节", EndFree-StartFree);



		MyFreeList = FreeList + FREELIST_INDEX(n);		//寻找对应的自由链表
		result = *MyFreeList;				//取到第一块自由链表的节点
		if (result == NULL)					//如果没有节点
		{

			void *tmp = Refill(ROUND_UP(n));			//进行从内存池中填充
			return tmp;							
		}

		*MyFreeList = result->FreeListLink;		//将剩下的放到自由链表上
		return result;							//返回取到的第一块内存。
	}
	static void DeAllocate(void *p, size_t n)
	{
		obj* tmp = (obj*) p;
		obj* volatile *MyFreeList;

		__TRACE_DEBUG("向二级空间配置器释放了%u个字节", n);
		__TRACE_DEBUG("内存池剩下了%u个字节", EndFree-StartFree);


		if (n > (size_t)__MAX_BYTES)	//如果大于128个字节，那么调用第一级分配器进行释放
		{
			MallocAllocTemplate<inst>::DeAllocate(p,n);
			return;
		}
		
		MyFreeList = FreeList + FREELIST_INDEX(n);		//寻找字节大小对应的自由链表
		tmp->FreeListLink = *MyFreeList;				//进行头插放回内存池
		*MyFreeList = tmp;
	}

private:
	static char *StartFree;			//记录内存池的开始位置
	static char *EndFree;			//记录内存池的结束位置
	static size_t HeapSize;			//
};


template<bool threads, int inst>
typename DefaultAllocTemplate<threads,inst>::
obj* volatile DefaultAllocTemplate<threads,inst>::
FreeList[__NFREELISTS] = { 0 };		//自由链表的声明。每个自由链表中存放的是对象的指针类型。

template<bool threads, int inst>
char *DefaultAllocTemplate<threads, inst>::StartFree = NULL;//记录内存池的开始位置

template<bool threads, int inst>
char *DefaultAllocTemplate<threads, inst>::EndFree = NULL;			//记录内存池的结束位置

template<bool threads, int inst>
size_t DefaultAllocTemplate<threads, inst>::HeapSize = 0;


#ifdef __USE_MALLOC_
typedef MacllocAllocTemptle<0> alloc;
#else
typedef DefaultAllocTemplate<0, 0> alloc;
#endif	//!__USE_MALLOC_


template<typename T,typename __Alloc>
class SimpleAlloc
{
public:
	static T*Allocate(size_t n)
	{
		if (n == 0)
			return NULL;
		else
			return (T*)__Alloc::Allocate(n*sizeof(T));
	}
	static T*Allocate(void)
	{
		return (T*)__Alloc::Allocate(sizeof(T));
	}
	static void Deallocate(T*p, size_t n)
	{
		if (p == NULL)
			return;
		__Alloc::DeAllocate(p, n*sizeof(T));
	}
	static void Deallocate(T* p)
	{
		__Alloc::DeAllocate(p, sizeof(T));
	}
};