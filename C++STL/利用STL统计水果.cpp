//��Ŀ��������һ����˾��Ҫͳ�ƴ��ϲ��ʲôˮ����Ȼ���Ա�������ˮ�����е�ǰ����



//���ǽ����˼·���ǣ����Ȱ�ÿ��Ա������ѡ��push_back��һ��vector���У�Ȼ������map�����ȥͳ�ƴ�������������ˣ���ô������++
//���û�г��֣���ô�ͽ��в����ȥ��Ȼ�����ǰ�map���ٴ��õ�����һ��vector���У�������µ�vector���ж��������������վͿ��ԾͿ���
//�ų�һ������Ҫ��Ķѡ�Ȼ������ȡ������ѵ�ǰ����ˮ���ͺ��ˡ������Ϳ��Եõ�������Ҫ��Ч���ˡ�

//�º������ṩ���������compare
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
	fruitselect.push_back("ƻ��");
	fruitselect.push_back("ƻ��");
	fruitselect.push_back("⨺���");
	fruitselect.push_back("⨺���");
	fruitselect.push_back("⨺���");
	fruitselect.push_back("⨺���");
	fruitselect.push_back("�㽶");
	fruitselect.push_back("�㽶");
	fruitselect.push_back("�㽶");
	fruitselect.push_back("�㽶");
	fruitselect.push_back("��");
	fruitselect.push_back("��");
	fruitselect.push_back("ƻ��");
	fruitselect.push_back("ƻ��");
	fruitselect.push_back("ƻ��");
	fruitselect.push_back("����");
	fruitselect.push_back("��");
	fruitselect.push_back("��");

	//���в��뵽map����ͳ�ƴ�����
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
	
	//Ҳ����ֱ������vector�Ĺ��캯������һ�ε��������䡣
	//std::vector<pair<string, int>> sortvector(fruitmap.begin(), fruitmap.end());
	
	//Ҳ����ֱ�Ӳ��������
	std::vector<map<string, int>::iterator> sortvector;
	
	
	std::vector<pair<string, int>> sortvector;
	//���в��뵽������
	// for (size_t i = 0; begin != end; i++)
	// {
		// sortvector.push_back(pair<string,int>(begin->first, begin->second));
		// begin++;
	// }
	
	sortvector.insert(sortvector.begin(),begin,end);
	
	
	//���ж�����
	make_heap(sortvector.begin(), sortvector.end(), comparepair());

	sort_heap(sortvector.begin(), sortvector.end(), comparepair());


	size_t k = 2;
	
	//����Top K
	for (int i = sortvector.size()-1; i >= 0 ; --i)
	{
		cout << sortvector[i].first <<" "<< sortvector[i].second <<" " <<endl;
	}

	

}