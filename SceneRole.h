#pragma once
#include <cstdint>
using namespace std;
namespace CL
{
class SceneRole
{
public:
	SceneRole(uint64_t id, uint16_t x, uint16_t y) :
		id_(id), x_(x), y_(y), 
		view_x_(1), view_y_(1) //Ĭ��x��y�������Ұ��С��ȣ�Ҳ���Բ����
	{}
	~SceneRole() {};

	uint64_t id() const { return id_; }

	uint16_t x() const { return x_; }
	void set_x(uint16_t x) { x_ = x; }

	uint16_t y() const { return y_; }
	void set_y(uint16_t y) { y_ = y; }

	uint16_t view_x() const { return view_x_; }
	void set_view_x(uint16_t radius) { view_x_ = radius; }

	uint16_t view_y() const { return view_y_; }
	void set_view_y(uint16_t radius) { view_y_ = radius; }

private:
	uint64_t id_;
	//����
	uint16_t x_;
	uint16_t y_;
	//uint16_t z_;
	//��Ұ�뾶
	uint16_t view_x_;
	uint16_t view_y_;
	//uint16_t view_z_;

};//class SceneRole
}//namespace CL

