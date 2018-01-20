#include "AOI.h"
#include "SceneRole.h"
//author: atao 1628025718@qq.com

CL::AOI::AOI(uint16_t width, uint16_t height) :
	width_(width), height_(height)
{
}


CL::AOI::~AOI()
{
}


void CL::AOI::enter(SceneRole* role)
{
	roles_.insert({ role->id(), role });
	//����listx_
	add_list(listx_, role->id(), role->x(), x2listx_);
	//����listy_
	add_list(listy_, role->id(), role->y(), y2listy_);
}

void CL::AOI::leave(uint64_t id)
{
	auto it = roles_.find(id);
	if (it != roles_.end())
	{
		Point x = it->second->x();
		Point y = it->second->y();
		//�뿪��Ӧ������
		leave_list(listx_, it->second->x(), it->second->id(), x2listx_);
		leave_list(listy_, it->second->y(), it->second->id(), y2listy_);
		//update roles_
		roles_.erase(id);
	}
	
}

//��������
void CL::AOI::add_list(list <Point2Id>& points, Id id, Point point, map <Point, list<Point2Id>::iterator>& maps)
{
	// ��¼�Ƿ����ݲ��뵽�˺��ʵ�λ��
	bool is_updated = false;
	//���������ҵ����ʵĵط����в���
	for (auto it = points.begin(); it != points.end(); ++it)
	{
		Point the_point = it->begin()->first;
		if (the_point == point) // ���뵽��ǰλ��
		{
			it->begin()->second.insert(id);
			is_updated = true;
			break;
		}
		else if (the_point > point) //���뵽��ǰλ�õ�ǰһ��λ��
		{
			Ids ids;
			Point2Id data;
			init_point2id(ids, data, id, point);
			data.insert({ point, ids });
			points.insert(it, data);
			is_updated = true;
			//��������
			maps.insert({ point, --it });
			break;
		}
		else //����������
		{
			continue;
		}
	}
	//������������е��ֵ������ô���뵽���
	if (!is_updated)
	{
		Ids ids;
		Point2Id data;
		init_point2id(ids, data, id, point);
		auto it = points.end();
		points.insert(it, data);
		maps.insert({ point, --it });
		is_updated = true;
	}
}

//�뿪
void CL::AOI::leave_list(list <Point2Id>& points, Point point, Id id, map <Point, list<Point2Id>::iterator>& maps)
{
	//�򵥵�ͨ��������ָ����λ����ɾ��
	//TODO ���ͨ��λ��ֱ��ӳ�䵽��Ӧ����Ľڵ㣬Ȼ��ֱ��ɾ��
	/*
	for (auto it = points.begin(); it != points.end(); ++it)
	{
		Point the_point = it->begin()->first;
		if (the_point == point)
		{
			it->begin()->second.erase(id);
			if (it->begin()->second.size() == 0) //�����������Ѿ�û�������ˣ��Ǿ�ɾ����
			{
				points.erase(it);
			}
			break;
		}
	}
	*/
	// ֱ��ʹ�����굽�����maps�����ٸ�������
	auto it = maps.find(point);
	if ( it != maps.end())
	{
		//ֱ���ҵ���Ӧ��listλ��
		auto list_it = it->second;
		Point the_point = list_it->begin()->first;
		if (the_point == point)
		{
			list_it->begin()->second.erase(id);
			if (list_it->begin()->second.size() == 0) //�����������Ѿ�û�������ˣ��Ǿ�ɾ����
			{
				points.erase(list_it);
				maps.erase(point);//ͬ��ɾ��ӳ���ϵ
			}
		}
	}
}

// �ƶ�
// �㷨���ṩ�ļ��ϲ�����
// ������ set_intersection
// ����:  set_union
// ���set_difference (A��B �� B��A�����岻һ��)
//��ȡ�뿪�ͽ�����Ұ�����
//�����ƶ��������Ұ��A���ƶ�֮ǰ����Ұ��B��
//��ôA-B�����½����Լ���Ұ�Ķ���
//B-A�����뿪��Ұ�Ķ���
//A+B������Ҫ֪ͨ�ƶ���Ϣ�ļ���
void CL::AOI::move(SceneRole* role, Point next_x, Point next_y)
{
	//����λ��
	Point old_x = role->x();
	Point old_y = role->y();
	Id id = role->id();

	if (old_x != next_x)
	{
		//����listx_
		leave_list(listx_, old_x, id, x2listx_);
		add_list(listx_, role->id(), next_x, x2listx_);
	}
	if (old_y != next_y)
	{
		leave_list(listy_, old_y, id, y2listy_);
		add_list(listy_, role->id(), next_y, y2listy_);
	}
	
	// ֪ͨ�뿪�����롢�ƶ���Ϣ
	Ids old_views, new_views;
	get_view(old_x, old_y, role, old_views);
	get_view(next_x, next_y, role, new_views);
	Ids leave_ids, enter_ids, move_ids;
	//֪ͨ�뿪�ļ���
	std::set_difference(old_views.begin(), old_views.end(),
		new_views.begin(), new_views.end(),
		std::inserter(leave_ids, leave_ids.begin()));
	//֪ͨ����ļ���
	std::set_difference(new_views.begin(), new_views.end(),
		old_views.begin(), old_views.end(),
		std::inserter(enter_ids, enter_ids.begin()));
	//֪ͨ�ƶ��ļ���
	std::set_union(new_views.begin(), new_views.end(),
		old_views.begin(), old_views.end(),
		std::inserter(move_ids, move_ids.begin()));
	//֪ͨ��Ӧ����Ϣ
	//...
	//....

}

void CL::AOI::init_point2id(Ids& ids, Point2Id& data, Id id, Point point)
{
	ids.insert(id);
	data.insert({ point, ids });
}

void CL::AOI::get_view(Point x, Point y, SceneRole* role, Ids& view_ids)
{
	Ids xids, yids;
	//x���Ϸ�Χ�ڵ����
	//Point x = role->x();
	auto xit = x2listx_.find(x);
	if (xit != x2listx_.end())
	{
		auto listx_node = xit->second; //����ڵ�,�ڵ���һ��map������ֻ��һ��key��key����x���꣬��Ӧһ�����id����
		auto the_x_node = listx_node;
		//����
		std::set_union(xids.begin(), xids.end(),
			listx_node->begin()->second.begin(), listx_node->begin()->second.end(),
			std::inserter(xids, xids.begin())); //���ⲿ�����id���ϼ���ids��
		while (++listx_node != listx_.end()) //����Һ��ʽڵ��е����id
		{
			Point the_x = listx_node->begin()->first;
			if (std::abs(x - the_x) <= role->view_x()) //����Ұ��Χ��
			{
				std::set_union(xids.begin(), xids.end(),
					listx_node->begin()->second.begin(), listx_node->begin()->second.end(),
					std::inserter(xids, xids.begin()));
			}
			else
			{
				break;
			}
		}
		listx_node = the_x_node;
		// ��Ҫ����listx_node�Ѿ�������ͷ�������
		if (listx_node != listx_.begin())
		{
			while (--listx_node != listx_.begin()) //��ǰ�Һ��ʽڵ��е����id
			{
				Point the_x = listx_node->begin()->first;
				if (std::abs(x - the_x) <= role->view_x()) //����Ұ��Χ��
				{
					std::set_union(xids.begin(), xids.end(),
						listx_node->begin()->second.begin(), listx_node->begin()->second.end(),
						std::inserter(xids, xids.begin()));
				}
				else
				{
					break;
				}
			}
		}
	}

	//y���Ϸ�Χ�ڵ����
	//Point y = role->y();
	auto yit = y2listy_.find(y);
	if (yit != y2listy_.end())
	{
		auto listy_node = yit->second; //����ڵ�,�ڵ���һ��map������ֻ��һ��key��key����y���꣬��Ӧһ�����id����
		auto the_y_node = listy_node;
		std::set_union(yids.begin(), yids.end(),
			listy_node->begin()->second.begin(), listy_node->begin()->second.end(),
			std::inserter(yids, yids.begin())); //���ⲿ�����id���ϼ���ids��
		while (++listy_node != listy_.end()) //����Һ��ʽڵ��е����id
		{
			Point the_y = listy_node->begin()->first;
			if (std::abs(y - the_y) <= role->view_y()) //����Ұ��Χ��
			{
				std::set_union(yids.begin(), yids.end(),
					listy_node->begin()->second.begin(), listy_node->begin()->second.end(),
					std::inserter(yids, yids.begin()));
			}
			else
			{
				break;
			}
		}
		listy_node = the_y_node;
		if (listy_node != listy_.begin())
		{
			while (--listy_node != listy_.begin()) //��ǰ�Һ��ʽڵ��е����id
			{
				Point the_y = listy_node->begin()->first;
				if (std::abs(y - the_y) <= role->view_y()) //����Ұ��Χ��
				{
					std::set_union(yids.begin(), yids.end(),
						listy_node->begin()->second.begin(), listy_node->begin()->second.end(),
						std::inserter(yids, yids.begin()));
				}
				else
				{
					break;
				}
			}
		}
	}

	//��x��y����Ұ�Ľ���
	std::set_intersection(xids.begin(), xids.end(), 
		yids.begin(), yids.end(), 
		std::inserter(view_ids, view_ids.begin()));
}