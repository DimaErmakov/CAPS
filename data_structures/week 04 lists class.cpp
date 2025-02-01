#include<iostream>
#include<list>

template<typename T> class List;

template<typename T>
class Node {
	friend class List<T>;
public:
	Node(const T& value) : data(value), next(NULL) {}
	void set_next(Node<T>* item) { next = item; }

private:
	T data;
	Node<T>* next;
};

template<typename T>
class List {
public:
	List() : first(NULL), last(NULL) {}

	void push_front(const T&);
	void push_back(const T&);
	bool is_empty() const;
	void print() const;

private:
	Node<T>* first;
	Node<T>* last;
};

int main() {
	//std::list<int>{};
	List<int> data;
	data.push_front(4);
	data.push_back(7);
	data.print();


}

template<typename T>
void List<T>::push_front(const T& item) {
	Node<T>* new_node = new Node<T>{ item };

	if (is_empty()) {
		first = new_node;
		last = new_node;
	}
	else {
		//(*new_node).next = first;
		new_node->set_next(first);
		first = new_node;
	}
}

template<typename T>
void List<T>::push_back(const T& item) {
	if (is_empty()) {
		push_front(item);
	}
	else {
		Node<T>* new_node = new Node<T>{ item };
		last->next = new_node;
		last = new_node;
	}
}

template<typename T>
bool List<T>::is_empty() const {
	return first == NULL;
}

template<typename T>
void List<T>::print() const {
	Node<T>* current = first;
	while (current != NULL) {
		std::cout << current->data << ' ';
		current = current->next;
	}
}