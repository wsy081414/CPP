//题目是现在有一个公司需要统计大家喜欢什么水果，然后给员工买这个水果当中的前两种



//我们解决的思路就是，首先把每个员工的所选择push_back到一个vector当中，然后利用map插入进去统计次数，如果出现了，那么次数就++
//如果没有出现，那么就进行插入进去。然后我们把map的再次拿到另外一个vector当中，对这个新的vector进行堆排序。这样，最终就可以就可以
//排出一个你所要求的堆。然后我们取出这个堆的前两个水果就好了。这样就可以得到最终想要的效果了。

//仿函数，提供给堆排序的compare
struct comparepair
{
	bool operator()(const pair<string, int>& a, const  pair<string, int>& b)
	{
		return a.second < b.second;
	}

};

void test()
{
	
	map<string, int> fruitmap;
	vector<string > fruitselect;
	map<string, int>::iterator it;
	fruitselect.push_back("苹果");
	fruitselect.push_back("苹果");
	fruitselect.push_back("猕猴桃");
	fruitselect.push_back("猕猴桃");
	fruitselect.push_back("猕猴桃");
	fruitselect.push_back("猕猴桃");
	fruitselect.push_back("香蕉");
	fruitselect.push_back("香蕉");
	fruitselect.push_back("香蕉");
	fruitselect.push_back("香蕉");
	fruitselect.push_back("梨");
	fruitselect.push_back("梨");
	fruitselect.push_back("苹果");
	fruitselect.push_back("苹果");
	fruitselect.push_back("苹果");
	fruitselect.push_back("桃子");
	fruitselect.push_back("梨");
	fruitselect.push_back("梨");

	//进行插入到map当中统计次数。
	for (size_t i = 0; i < fruitselect.size(); i++)
	{
	/*	if (( it=fruitmap.find(fruitselect[i])) != fruitmap.end())
		{
			it->second++;
		}
		fruitmap.insert(pair<string ,int>(fruitselect[i],1));*/

		fruitmap[fruitselect[i]]++;
	}
	fruitselect.clear();

	map<string, int>::iterator begin = fruitmap.begin();
	map<string, int>::iterator end = fruitmap.end();
	
	//也可以直接利用vector的构造函数构造一段迭代器区间。
	//std::vector<pair<string, int>> sortvector(fruitmap.begin(), fruitmap.end());
	
	//也可以直接插入迭代器
	std::vector<map<string, int>::iterator> sortvector;
	
	
	std::vector<pair<string, int>> sortvector;
	//进行插入到堆排序
	// for (size_t i = 0; begin != end; i++)
	// {
		// sortvector.push_back(pair<string,int>(begin->first, begin->second));
		// begin++;
	// }
	
	sortvector.insert(sortvector.begin(),begin,end);
	
	
	//进行堆排序
	make_heap(sortvector.begin(), sortvector.end(), comparepair());

	sort_heap(sortvector.begin(), sortvector.end(), comparepair());


	size_t k = 2;
	
	//访问Top K
	for (int i = sortvector.size()-1; i >= 0 ; --i)
	{
		cout << sortvector[i].first <<" "<< sortvector[i].second <<" " <<endl;
	}

	

}