#pragma once
#include <iostream>
#include <string>
#include <vector>

class Sales_data {
	// ��Ԫ��ֻ�ܳ��������ڲ���������λ�ò��ޡ���������ĳ�Ա��Ҳ��������������ʿ��Ƽ����Լ����
	// ��Ԫ����������ָ���˷���Ȩ�ޣ�����ͨ������ĺ�����������Ҫ�����ⵥ��������һ����Ԫ������
	friend Sales_data add(const Sales_data&, const Sales_data&);
	friend std::istream& read(std::istream&, Sales_data&);
	friend std::ostream& print(std::ostream&, const Sales_data&);
public:
	// ���캯��
	// ʹ��ί�й��캯������ͨ�õĲ��ֽ�һ����װ����
	// ��ί�й��캯��ʹ�ö�Ӧ��ʵ�γ�ʼ����Ա
	Sales_data(std::string s, unsigned cnt, double price) :bookNo(s), units_sold(cnt), revenue(cnt* price) {};
	// ���๹�캯��ȫ��ί�и���һ�����캯��
	Sales_data() : Sales_data("", 0, 0) {};
	Sales_data(std::string s) : Sales_data(s, 0, 0) {};
	Sales_data(std::istream& is) : Sales_data() { read(is, *this); }

	//Sales_data() = default;
	//Sales_data(const std::string& s) : bookNo(s) {};
	//Sales_data(const std::string& s, unsigned n, double p) : bookNo(s), units_sold(n), revenue(p* n) {};
	//Sales_data(std::istream&);

	// ���� Sales_data ����Ĳ���
	std::string isbn() const { return bookNo; }
	Sales_data& combine(const Sales_data&);

private:
	double avg_price() const {
		if (units_sold)
			return revenue / units_sold;
		else
			return 0;
	}

	// ���ݳ�Ա
	std::string bookNo;
	unsigned units_sold = 0;
	double revenue = 0.0;
	
};


// Sales_data �ķǳ�Ա�ӿں���
Sales_data add(const Sales_data&, const Sales_data&);
std::ostream& print(std::ostream&, const Sales_data&);
std::istream& read(std::istream&, Sales_data&);

Sales_data& Sales_data::combine(const Sales_data& rhs) {
	units_sold += rhs.units_sold;
	revenue += rhs.revenue;
	return *this;
}


std::istream& read(std::istream& is, Sales_data& item) {
	double price = 0;
	is >> item.bookNo >> item.units_sold >> price;
	item.revenue = price * item.units_sold;
	return is;
}

std::ostream& print(std::ostream& os, const Sales_data& item) {
	os << item.isbn() << " " << item.units_sold << " "
		<< item.revenue << " " << item.avg_price();
	return os;
}

Sales_data add(const Sales_data& lhs, const Sales_data& rhs) {
	Sales_data sum = lhs;
	sum.combine(rhs);
	return sum;
}

class Screen {
	// Window_mgr �ĳ�Ա���Է��� Screen ���˽�в���
	friend class Window_mgr;
public:
	using pos = std::string::size_type;
	Screen() = default;
	Screen(pos ht, pos wd, char c) : height(ht), width(wd), contents(ht* wd, c) {};
	char get() const { return contents[cursor]; }; // ��ʽ���������������ڲ��ĳ�Ա�����Զ� inline��
	inline char get(pos ht, pos wd) const; // ��ʽ���������ֻ�����ⲿ����ĵط�˵�� inline��ʹ���������⡣
	Screen& move(pos r, pos c); // ����֮����Ϊ����
	void some_member() const;
	Screen& set(char);
	Screen& set(pos, pos, char);
	Screen& display(std::ostream& os) { do_display(os); return *this; }
	const Screen& display(std::ostream& os) const { do_display(os); return *this; }

private:
	mutable size_t access_ctr; // ��ʹ��һ�� const ������Ҳ�ܱ��޸�
	pos cursor = 0;
	pos height = 0, width = 0;
	std::string contents;
	void do_display(std::ostream& os) const { os << contents; }
};

inline Screen& Screen::move(pos r, pos c) {
	pos row = r * width;
	cursor = row + c;
	return *this;
}

char Screen::get(pos r, pos c) const {
	pos row = r * width;
	return contents[row + c];
}

void Screen::some_member() const {
	++access_ctr; // ����һ������ֵ�����ڼ�¼��Ա���������õĴ���
	// �ó�Ա��Ҫ��ɵ���������
}

inline Screen& Screen::set(char c) {
	contents[cursor] = c;
	return *this;
}

inline Screen& Screen::set(pos r, pos col, char ch) {
	contents[r * width + col] = ch;
	return *this;
}


class Window_mgr {
public:
	// ������ÿ����Ļ�ı��
	using ScreenIndex = std::vector<Screen>::size_type;
	// ���ձ�Ž�ָ���� Screen ��Ϊ�հ�
	void clear(ScreenIndex);
	// �򴰿����һ�� Screen�����������ı��
	ScreenIndex addScreen(const Screen&);
private:
	// ��� Window_mgr ׷�ٵ� Screen
	// Ĭ������£�һ�� Window_mgr ����һ����׼�ߴ�Ŀհ� Screen
	std::vector<Screen> screens{ Screen(24, 80, ' ') };
};

void Window_mgr::clear(ScreenIndex i) {
	// s ��һ�� Screen �����ã�ָ����������յ��Ǹ���Ļ
	Screen& s = screens[i];
	// ��ѡ���� Screen ��Ϊ�հ�
	s.contents = std::string(s.height * s.width, ' ');
}

// ��Ϊ��ǰ�� Window_mgr ������֮�⣬����ʹ�����������ñ�����֪�� ScreenIndex ������ Window_mgr �������ڵ�
Window_mgr::ScreenIndex Window_mgr::addScreen(const Screen& s) {
	screens.push_back(s);
	return screens.size() - 1;
}