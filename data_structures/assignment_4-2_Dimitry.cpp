#include <iostream>
#include <vector>
#include <list>
#include <cstring> 
struct Item
{
	const char* interrupt;
	int priority;
};
class Queue // index 0 is the highest priority
{
public:
	void push(const char* interrupt, int priority)
	{
		if (strcmp(interrupt, "none") == 0)
		{
			return;
		}
		if (this->is_empty())
		{
			data.push_back({ interrupt, priority });
			return;
		}
		for (auto iter = data.begin(); iter != data.end(); iter++)
		{
			if (priority < iter->priority)
			{
				data.insert(iter, { interrupt, priority });
				return;
			}
		}
		data.push_back({ interrupt, priority }); // lowest priority
	}
	void pop()
	{
		if (this->is_empty())
		{
			std::cout << "Queue is empty" << std::endl;
			return;
		}
		std::cout << "Processing " << data.front().interrupt << std::endl;
		data.pop_front();
	}
	bool is_empty()
	{
		return data.size() == 0;
	}
	void clear()
	{
		data.clear();
	}
	void print()
	{
		std::cout << "Queue:" << std::endl;
		for (auto iter = data.begin(); iter != data.end(); iter++)
		{
			std::cout << iter->interrupt << " " << iter->priority << std::endl;
		}
	}

private:
	std::list<Item> data;
};
class Handler
{
public:
	void poll(const char* interrupt, int priority)
	{
		queue.push(interrupt, priority);
	}
	void process()
	{
		queue.pop();
	}
	void print()
	{
		queue.print();
	}
private:
	Queue queue;
};
int main()
{
	Handler handler;

	handler.poll("timer1", 2);
	handler.poll("timer2", 4);
	handler.print();
	handler.process();
	handler.print();
	handler.poll("serial", 3);
	handler.poll("none", 0);
	handler.print();
	handler.process();
	handler.poll("external1", 2);
	handler.poll("timer1", 5);
	handler.print();
	handler.process();
	handler.poll("none", 0);
	handler.poll("external2", 3);
	handler.print();
	handler.process();
	handler.poll("serial", 1);
	handler.poll("timer0", 3);
	handler.print();
	handler.process();
	handler.poll("none", 0);
	handler.poll("external1", 4);
	handler.print();
	handler.process();
	handler.poll("serial", 2);
	handler.print();
	handler.process();
	handler.process();
	handler.process();
	handler.process();
	handler.process();

}