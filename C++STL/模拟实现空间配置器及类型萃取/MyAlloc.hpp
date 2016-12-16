
#pragma once
#define _CRT_SECURE_NO_WARNINGS 1

#include<iostream>
#include<cstdarg>
#include<cstdlib>

using namespace std;



// Trace ����

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
// ���ڵ���׷�ݵ�trace log
inline static void __trace_debug(const char* function,
	const char * filename, int line, char* format, ...)
{
	// ��ȡ�����ļ�
#ifdef __DEBUG__

	// ������ú�������Ϣ
	fprintf(stdout, "�� %s:%d��%s", GetFileName(filename).c_str(), line, function);
	fprintf(fOut, "�� %s:%d��%s", GetFileName(filename).c_str(), line, function);

	// ����û����trace��Ϣ
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







//�ڴ����ĵ�һ�����������ڵ�һ�����У������жϵ��õ�һ�����������ǵڶ���������������ǵڶ�����β�����
template<int inst>
class MallocAllocTemplate
{
	//����һ������ָ������
	typedef void(*_FuncP)();
private:
	static _FuncP MallocAllocOOMHandler;	//�����ڴ治����һ�����
	static void *OOMMalloc(size_t);		//�����ڴ治���ĺ���
public:
	static void *Allocate(size_t n)
	{

		__TRACE_DEBUG("��һ���ռ�������������%u���ֽ�", n);
		void *result = malloc(n);	//���з���n���ڴ�ռ�
		if (result == NULL)			//�ж��Ƿ����ɹ�
			result = OOMMalloc(n);	//�ڴ���䲻�ɹ�������OOMMalloc�鿴�Ƿ���Բ���ϵͳ�ͷ��ڴ���䣬����޷����䣬�׳��쳣��
		return result;
	}
	static void DeAllocate(void *p, size_t n)
	{
		__TRACE_DEBUG("һ���ռ��������ͷ���%u���ֽ�", n);
		free(p);		//��һ���ռ�������ֱ��ʹ��free
	}

	static _FuncP SetMallocHandler(_FuncP f)		//�����ļ������ͨ�������������ʹ֪������ϵͳ���Ƿ��л����Է�����ڴ档
	{
		_FuncP old = MallocAllocOOMHandler;			//
		MallocAllocOOMHandler = f;					//�����ļ����
		return old;									//���ؾɵģ�����ϵͳ����д����
	}
};


template<int inst>
typename MallocAllocTemplate<inst>::
_FuncP MallocAllocTemplate<inst>::
MallocAllocOOMHandler = 0;	//��ʼ��ָ��Ϊ��

template<int inst>
void * MallocAllocTemplate<inst>::OOMMalloc(size_t n)		//�ڴ治���ĺ���
{
	_FuncP MyMallocHandler;
	void * result;
	while (1)
	{
		MyMallocHandler = MallocAllocOOMHandler;		//
		if (MyMallocHandler == 0)						//���ļ�����Ƿ�Ϊ�գ�Ϊ�վ��׳��쳣
			throw bad_alloc();							//��Ϊ�գ�˵������ϵͳ�л��п����ڴ����ͨ���ͷ��������з��䡣

		//��������еľ���set_new_handler���ƣ�����ʹ�ø���Ŀռ��ɿɷ���Ŀռ䡣
		//������ܾͻ������ڴ棬�ﵽ������ܡ�
		(*MyMallocHandler)();							//�ͷ�һЩ�޹��ڴ棬��������


		result = malloc(n);
		if (result)										//�������ɹ�����ô�ͷ���
			return result;								//���򣬼�������ѭ���鿴
	}
}




//�����ռ�����������
template<bool threads,int inst>
class DefaultAllocTemplate
{
private:
	enum { __ALIGN = 8 };								//���Ķ�����
	enum { __MAX_BYTES = 128 };							//����ֽ���
	enum { __NFREELISTS = __MAX_BYTES / __ALIGN };		//���������������

	//�õ�һ��bytes�����һ��8������������
	static size_t ROUND_UP(size_t bytes) {
		return (((bytes)+__ALIGN - 1) & ~(__ALIGN - 1));
	}

	//����bytesӦ�����ڵڼ�����������
	static  size_t FREELIST_INDEX(size_t bytes) {
		return (((bytes)+__ALIGN - 1) / __ALIGN - 1);
	}
	//ÿһ�����ڴ�ڵ�Ľṹ
	union obj
	{
		union obj* FreeListLink;		//���������ָ����һ����ָ�룬
		char client_data;				//�ṩ���ͻ��˲鿴
	};

	static obj* volatile FreeList[__NFREELISTS];		//���������������ÿ�����������д�ŵ��Ƕ����ָ�����͡�
	//���ռ�������
	//��ʱ��n���Ѿ�Ϊ8�������������ǲ�����ROUND_UP����
	//������������������Ƿ���n�������С���ڴ棬Ȼ�������������
	static void*  Refill(size_t n /*n�Ƕ���Ĵ�С*/)
	{
		int nodjs = 20;		//Ĭ�ϴ��ڴ����ȡ��20���ڴ�ڵ㡣



		char * chunk = ChunkAlloc(n, nodjs);		//���������ȡ���ڴ��������鿴����ȡ�����ڴ���Ƕ��

		obj *volatile *MyFreeList = NULL;

		obj *result = NULL;

		//�����ٻ����һ���ڴ�飬ֱ�ӷ��ظ��ͻ���                                                        
		if (nodjs == 1)		
		{
			return chunk;
		}
		//������˶���ڴ�ģ�ʱ����е������ѵ�һ�鷵�ظ��ͻ��ˣ�Ȼ��ʣ�µķŵ�������������
	
		MyFreeList = FreeList + FREELIST_INDEX(n);		//�ҵ����Ǹ����������·��ڴ�顣

		//ע�⣬ÿ��ȡҲֻ�Ƕ���Ĵ�С���ڴ�ռ�
		result = (obj*)chunk;

		obj* Cur=NULL;
		obj* Next = NULL;
		//��������Ҫ�Ĵ�С���ڴ����ڴ�ص���ȡ������
		*MyFreeList=Next = (obj *)(chunk + n);

		Cur = Next;
		//����ȡ�����ڴ����з����Ϊ�����������ʽ
		size_t i = 1;

		while (1)
		{
			Next = (obj *)((char *)Next + n);	//ȡ����һ������С���ڴ�顣

			//�������һ���ڵ������������ڵ�
			if (nodjs - 1 == i)
			{
				Cur->FreeListLink = NULL;		//���һ���ڵ㣬�����ſա�
				break;
			}
			else
				Cur->FreeListLink = Next;		//�����ڵ㣬���Ǹýڵ����һ���ڵ��Next��
			
			Cur = Next;		//����Cur��

			i++;
		}

		return result;	//������һ��ռ䷵�ء�
	}
	//�������ڴ�������ڴ�飬�������յ�nobjs�Ĳ�ͬ�����ؿ��ٵ�����Ĳ�ͬ
	static char* ChunkAlloc(size_t size, int &nobjs)
	{
		char* result;
		size_t totalbytes = nobjs*size;
		size_t leftbytes = EndFree - StartFree;

		if (totalbytes <= leftbytes)
		{
			__TRACE_DEBUG("�ڴ�������㹻%u��", nobjs);

			result = StartFree;
			StartFree += totalbytes;
			return result;
		}
		if (leftbytes >= size)
		{
			__TRACE_DEBUG("�ڴ����ֻ�ܹ�%u��", nobjs);

			nobjs = leftbytes / size;
			result = StartFree;
			StartFree += nobjs*size;
			return result;
		}
		else
		{
			size_t bytestoget = 2 * totalbytes + ROUND_UP(HeapSize >> 4);
			__TRACE_DEBUG("�ڴ����ϵͳ����%u���ֽ�",bytestoget);

			if (leftbytes > 0)
			{
				__TRACE_DEBUG("�ڴ��ʣ��%u���ֽڣ��ŵ���������%u����", bytestoget, FREELIST_INDEX(leftbytes));

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
		obj* volatile*MyFreeList;	//ָ��һ����������ڵ�
		obj* result;
		//������128���ֽڵ�ʱ��ȥ���õ�һ��������
		if (n > (size_t)__MAX_BYTES)
			return MallocAllocTemplate<inst>::Allocate(n);

		__TRACE_DEBUG("������ռ�������������%u���ֽ�", n);
		__TRACE_DEBUG("�ڴ��ʣ����%u���ֽ�", EndFree-StartFree);



		MyFreeList = FreeList + FREELIST_INDEX(n);		//Ѱ�Ҷ�Ӧ����������
		result = *MyFreeList;				//ȡ����һ����������Ľڵ�
		if (result == NULL)					//���û�нڵ�
		{

			void *tmp = Refill(ROUND_UP(n));			//���д��ڴ�������
			return tmp;							
		}

		*MyFreeList = result->FreeListLink;		//��ʣ�µķŵ�����������
		return result;							//����ȡ���ĵ�һ���ڴ档
	}
	static void DeAllocate(void *p, size_t n)
	{
		obj* tmp = (obj*) p;
		obj* volatile *MyFreeList;

		__TRACE_DEBUG("������ռ��������ͷ���%u���ֽ�", n);
		__TRACE_DEBUG("�ڴ��ʣ����%u���ֽ�", EndFree-StartFree);


		if (n > (size_t)__MAX_BYTES)	//�������128���ֽڣ���ô���õ�һ�������������ͷ�
		{
			MallocAllocTemplate<inst>::DeAllocate(p,n);
			return;
		}
		
		MyFreeList = FreeList + FREELIST_INDEX(n);		//Ѱ���ֽڴ�С��Ӧ����������
		tmp->FreeListLink = *MyFreeList;				//����ͷ��Ż��ڴ��
		*MyFreeList = tmp;
	}

private:
	static char *StartFree;			//��¼�ڴ�صĿ�ʼλ��
	static char *EndFree;			//��¼�ڴ�صĽ���λ��
	static size_t HeapSize;			//
};


template<bool threads, int inst>
typename DefaultAllocTemplate<threads,inst>::
obj* volatile DefaultAllocTemplate<threads,inst>::
FreeList[__NFREELISTS] = { 0 };		//���������������ÿ�����������д�ŵ��Ƕ����ָ�����͡�

template<bool threads, int inst>
char *DefaultAllocTemplate<threads, inst>::StartFree = NULL;//��¼�ڴ�صĿ�ʼλ��

template<bool threads, int inst>
char *DefaultAllocTemplate<threads, inst>::EndFree = NULL;			//��¼�ڴ�صĽ���λ��

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