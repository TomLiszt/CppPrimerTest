#pragma once
#include <iostream>
#include <string>
#include <vector>

class Sales_data {
	// 友元，只能出现在类内部，但具体位置不限。它不是类的成员，也不受所在区域访问控制级别的约束。
	// 友元的声明仅仅指定了访问权限，并非通常意义的函数声明，需要在类外单独再声明一次友元函数。
	friend Sales_data add(const Sales_data&, const Sales_data&);
	friend std::istream& read(std::istream&, Sales_data&);
	friend std::ostream& print(std::ostream&, const Sales_data&);
public:
	// 构造函数
	// 使用委托构造函数，把通用的部分进一步封装起来
	// 非委托构造函数使用对应的实参初始化成员
	Sales_data(std::string s, unsigned cnt, double price) :bookNo(s), units_sold(cnt), revenue(cnt* price) {};
	// 其余构造函数全部委托给另一个构造函数
	Sales_data() : Sales_data("", 0, 0) {};
	Sales_data(std::string s) : Sales_data(s, 0, 0) {};
	Sales_data(std::istream& is) : Sales_data() { read(is, *this); }

	//Sales_data() = default;
	//Sales_data(const std::string& s) : bookNo(s) {};
	//Sales_data(const std::string& s, unsigned n, double p) : bookNo(s), units_sold(n), revenue(p* n) {};
	//Sales_data(std::istream&);

	// 关于 Sales_data 对象的操作
	std::string isbn() const { return bookNo; }
	Sales_data& combine(const Sales_data&);

private:
	double avg_price() const {
		if (units_sold)
			return revenue / units_sold;
		else
			return 0;
	}

	// 数据成员
	std::string bookNo;
	unsigned units_sold = 0;
	double revenue = 0.0;
	
};


// Sales_data 的非成员接口函数
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
	// Window_mgr 的成员可以访问 Screen 类的私有部分
	friend class Window_mgr;
public:
	using pos = std::string::size_type;
	Screen() = default;
	Screen(pos ht, pos wd, char c) : height(ht), width(wd), contents(ht* wd, c) {};
	char get() const { return contents[cursor]; }; // 隐式内联（定义在类内部的成员函数自动 inline）
	inline char get(pos ht, pos wd) const; // 显式内联，最好只在类外部定义的地方说明 inline，使类更容易理解。
	Screen& move(pos r, pos c); // 能在之后被设为内联
	void some_member() const;
	Screen& set(char);
	Screen& set(pos, pos, char);
	Screen& display(std::ostream& os) { do_display(os); return *this; }
	const Screen& display(std::ostream& os) const { do_display(os); return *this; }

private:
	mutable size_t access_ctr; // 即使在一个 const 对象内也能被修改
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
	++access_ctr; // 保存一个计数值，用于记录成员函数被调用的次数
	// 该成员需要完成的其他工作
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
	// 窗口中每个屏幕的编号
	using ScreenIndex = std::vector<Screen>::size_type;
	// 按照编号将指定的 Screen 置为空白
	void clear(ScreenIndex);
	// 向窗口添加一个 Screen，并返回它的编号
	ScreenIndex addScreen(const Screen&);
private:
	// 这个 Window_mgr 追踪的 Screen
	// 默认情况下，一个 Window_mgr 包含一个标准尺寸的空白 Screen
	std::vector<Screen> screens{ Screen(24, 80, ' ') };
};

void Window_mgr::clear(ScreenIndex i) {
	// s 是一个 Screen 的引用，指向我们想清空的那个屏幕
	Screen& s = screens[i];
	// 将选定的 Screen 置为空白
	s.contents = std::string(s.height * s.width, ' ');
}

// 因为当前在 Window_mgr 作用域之外，必须使用类名才能让编译器知道 ScreenIndex 是属于 Window_mgr 作用域内的
Window_mgr::ScreenIndex Window_mgr::addScreen(const Screen& s) {
	screens.push_back(s);
	return screens.size() - 1;
}