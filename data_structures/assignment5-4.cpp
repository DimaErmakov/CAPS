#include <iostream>
#include <vector>
#include <string>
#include <stack>
/*Write a C++ program that performs a depth-first search on an arbitrary graph. Represent the
graph in adjacency matrix form. You may use a two-dimensional array or a vector of vectors to represent
this matrix. Your output should be the order of the nodes that are visited in the graph*/

void dfs(const std::vector<std::vector<int>>& graph, int start_node,
	std::vector<bool>& visited, const std::vector<char>& nodes) {
	std::stack<int> stack;
	stack.push(start_node);
	while (!stack.empty()) {
		int current_node = stack.top();
		stack.pop(); // A B D E C F
		if (!visited[current_node]) {
			std::cout << nodes[current_node] << " ";
			visited[current_node] = true;
			for (int neighbor = graph.size() - 1; neighbor >= 0; --neighbor) {
				if (graph[current_node][neighbor] == 1 && !visited[neighbor]) {
					//std::cout << neighbor << " ";
					//std::cout << graph[current_node][neighbor] << " " << nodes[current_node];
					// if there is an edge and the neighbor has not been visited
					stack.push(neighbor); // add the neighbor to the stack
				}
			}
		}
	}
}

int main() {
	std::vector<std::vector<int>> graph = {
		// A, B, C, D, E, F
		{0, 1, 1, 0, 0, 1}, // A
		{1, 0, 0, 1, 1, 1}, // B
		{1, 0, 0, 0, 1, 1}, // C
		{0, 1, 0, 0, 1, 0}, // D
		{0, 1, 1, 1, 0, 1}, // E
		{1, 1, 1, 0, 1, 0}  // F
	};
	std::vector<char> nodes = { 'A', 'B', 'C', 'D', 'E', 'F' };
	std::vector<bool> visited{ false, false, false, false, false, false };
	std::cout << "Depth-First Search Order:\n";
	for (int i = 0; i < 6; ++i) {
		if (!visited[i]) {
			dfs(graph, i, visited, nodes); // start dfs from node i
		}
	}
	return 0;
}