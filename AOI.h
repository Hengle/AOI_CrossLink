#pragma once
//author: atao 1628025718@qq.com
#include <cstdint>
#include <list>
#include <set>
#include <map>
#include <iostream>
#include <algorithm>
#include <iterator>
using namespace std;
namespace CL
{
class SceneRole;
class AOI
{
	typedef uint16_t Point;
	typedef uint64_t Id;
	typedef set<Id> Ids;
	typedef map<Point, Ids> Point2Id;
public:
	AOI(uint16_t width, uint16_t height);
	~AOI();

	//API
	void enter(SceneRole*);
	void leave(uint64_t);
	void move(SceneRole*, Point next_x, Point next_y);
	//debug
	void stat()
	{
		cout << "roles size:" << roles_.size() << endl;
		cout << "listx size:" << listx_.size() << endl;
		cout << "listy size:" << listy_.size() << endl;
	}
private:
	//��������
	void add_list(list <Point2Id>&, Id id, Point p, map <Point, list<Point2Id>::iterator>&);
	//�뿪����
	void leave_list(list <Point2Id>&, Point, Id, map <Point, list<Point2Id>::iterator>&);
	//��ʼ������ڵ�Ԫ��
	void init_point2id(Ids&, Point2Id&, Id id, Point point);
	//��Ұ���
	void CL::AOI::get_view(Point x, Point y, SceneRole* role, Ids& view_ids);
private:
	Point width_;
	Point height_;
	//x,y�����Ӧ�����id����
	list <Point2Id> listx_;
	list <Point2Id> listy_;
	//�������
	map <Id, SceneRole*> roles_;
	//������������Ϊlistx_, listy_�������Ч�ʺܵͣ������ṩx����Ӧ�����������mapӳ�䡣
	map <Point, list<Point2Id>::iterator> x2listx_;
	map <Point, list<Point2Id>::iterator> y2listy_;

};//class AOI
}//namespace CL

