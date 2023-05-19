#include <iostream>
#include <functional>
#include <string>
#include <sstream>
#include "Tree.cpp"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

const float RAD = 35;
int depth;

Tree* max_of_three(Tree* mas[3])
{
	Tree* max = new Tree;
	max = mas[0];

	if (mas[1]->data > max->data)
		max = mas[1];
	if (mas[2]->data > max->data)
		max = mas[2];

	return max;
}

Tree* Tree::maximum()
{
	Tree* mas[3];
	mas[0] = this;

	if (left != nullptr)
		mas[1] = left->maximum();
	else
		mas[1] = this;

	if (right != nullptr)
		mas[2] = right->maximum();
	else
		mas[2] = this;

	return max_of_three(mas);
}

void count_levels(Tree* root)
{
	if (root != nullptr)
	{
		if (root->parent != nullptr)
			root->level = root->parent->level + 1;

		if (depth < root->level)
			depth = root->level;

		count_levels(root->left);
		count_levels(root->right);
	}
}

void calculate_coords(Tree* node)
{
	if (node != nullptr)
	{
		if (node->parent != nullptr)
		{
			if (node->level == 2)
				node->x = node->x + node->state * (pow(2, depth - 1) / 2) * 40;
			else
				node->x = node->parent->x + node->state * (pow(2, depth - 1) / pow(2, node->level - 1)) * 40;
			node->y = node->parent->y + 80;
		}

		if (node->left != nullptr)
		{
			node->left->state = -1;
			calculate_coords(node->left);
		}

		if (node->right != nullptr)
		{
			node->right->state = 1;
			calculate_coords(node->right);
		}
	}
}

void draw_lines(Tree* node, RenderWindow& window)
{
	if (node != nullptr)
	{
		if (node->parent != nullptr)
		{
			vector<Vertex> line =
			{
			Vertex(Vector2f(node->parent->x + RAD, node->parent->y + RAD), Color::Black),
			Vertex(Vector2f(node->x + RAD, node->y + RAD), Color::Black)
			};
			window.draw(&line[0], line.size(), Lines);
		}

		if (node->left != nullptr)
			draw_lines(node->left, window);

		if (node->right != nullptr)
			draw_lines(node->right, window);
	}
}

void draw_nodes(Tree* node, RenderWindow& window)
{
	if (node != nullptr)
	{
		CircleShape circle(RAD);
		circle.setPosition(node->x, node->y);
		circle.setFillColor(Color::White);
		circle.setOutlineColor(Color::Black);
		circle.setOutlineThickness(3);

		Font font;
		font.loadFromFile("ofont.ru_a_CampusOtl.ttf");

		stringstream txt;
		txt << node->data;
		Text text(txt.str(), font, 35);
		text.setPosition(node->x + RAD / 2, node->y + RAD / 2);
		text.setFillColor(Color::Black);

		Vector2i mousePos = Mouse::getPosition(window);
		if (circle.getGlobalBounds().contains(mousePos.x, mousePos.y))
		{
			circle.setFillColor(Color{ 145, 230, 255 });
		}
		else
		{
			circle.setFillColor(Color::White);
		}

		window.draw(circle);
		window.draw(text);

		if (node->left != nullptr)
			draw_nodes(node->left, window);

		if (node->right != nullptr)
			draw_nodes(node->right, window);
	}
}

void draw(Tree* tree)
{
	RenderWindow MainWindow(VideoMode(1000, 500), "Our tree");
	depth = 0;

	count_levels(tree);
	calculate_coords(tree);

	while (MainWindow.isOpen())
	{
		Event event;

		while (MainWindow.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				MainWindow.close();
				break;
			}
		}

		MainWindow.clear(Color::White);
		draw_lines(tree, MainWindow);
		draw_nodes(tree, MainWindow);
		MainWindow.display();
	}
}

int main()
{
	setlocale(LC_ALL, "RUS");

	Tree* bush = new Tree;

	bush->generate();
	cout << "our tree: " << endl;
	draw(bush);

	/*bush->maximum();*/
	cout << "Максимальный элемент дерева: " << bush->maximum() << endl;
	draw(bush);

	Tree* search_bush = bush->convert_into_search();
	search_bush->generate_search();
	cout << "Преобразование в дерево поиска: " << endl;
	draw(search_bush);

	return 0;

}