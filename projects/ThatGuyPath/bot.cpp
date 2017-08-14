#include "bot.h"
#include "time.h"

extern "C"
{
	BotInterface27 BOT_API *CreateBot27()
	{
		return new Blank();
	}
}

Blank::Blank()
{
	m_rand(time(0)+(int)this);
}

Blank::~Blank()
{
}

void Blank::init(const BotInitialData &initialData, BotAttributes &attrib)
{
	m_initialData = initialData;
	attrib.health=1.0;
	attrib.motor=1.0;
	attrib.weaponSpeed=1.0;
	attrib.weaponStrength=1.0;
	dir.set(1, 0);
	m_map.init(initialData.mapData.width, initialData.mapData.height);
	for (int y = 0; y<m_map.m_height; ++y)
	{
		for (int x = 0; x<m_map.m_width; ++x)
		{
			m_map.getNode(NodePos(x, y)).wall = m_initialData.mapData.data[x + y*m_map.m_width].wall;
		}
	}
}

void Blank::update(const BotInput &input, BotOutput27 &output)
{
	output.lines.clear();
	output.text.clear();
	m_map.draw(output.lines, output.text, true, true, true);
	output.moveDirection = dir;
	output.motor = 1.0;
	output.lookDirection.set(0,1);
	output.action = BotOutput::scan;
	kf::Vector2 delta = target - input.position;


		setPosition();
		pathFind(NodePos(target.x, target.y));
}

void Blank::pathFind(NodePos startOfPath)
{
	//Map holds data.
	//	NodePos can identify a node in the map.
	//	std::vector<NodePos> openList;
	std::vector<NodePos> openList;

	//To find a path :
	//Clear map data.
	m_map.clear();

	//	pathFound = false
	bool pathFound = false;

	//	Push start node into openList.
	openList.push_back(startOfPath);

	//	While(openList is not empty) // if it's empty, there's no valid path
	while (openList.size() != 0 && pathFound == false)
	{
		//	Find node in openList with the smallest F value.
		NodePos currentPosition = openList.front();
		Node & currentNode = m_map.getNode(currentPosition);
		for (int i = 1; i < openList.size(); i++)
		{
			if (m_map.getNode(openList[i]).f < m_map.getNode(currentPosition).f)
			{
				currentPosition = openList[i];
			}
		}
		//	For oy = -1 to 1 // offset y
		for (int oy = -1; oy , 2; oy++)
		{
			//	For ox = -1 to 1 // offset x
			for (int ox = -1; ox < 2; ox++)
			{
				//	If ox == 0 and oy == 0 then continue // don't look at itself
				if (ox == 0 && oy == 0 || ox != 0 && oy != 0)
				{
					continue;
				}
				//	Adjacent nodepos is current nodepos + (x, y)
				NodePos adjacentPosition = currentPosition + NodePos(ox, oy);
				Node & adjacentNode = m_map.getNode(adjacentPosition);
				//	New G = current node G + adjacent node C.
				int newG = currentNode.g + adjacentNode.c;
				//	If adjacent node is closed or a wall
				if (adjacentNode.wall || adjacentNode.state == Node::StateClosed)
				{
					//	Do nothing.
				}
				//	Else If adjacent node is open and new g < adjacent g
				else if (adjacentNode.state == Node::StateOpen || newG < adjacentNode.g)
				{
					//	adjacent G = new G
					adjacentNode.g = newG;
					//	adjacent h = heuristic
					adjacentNode.h = 0;
					//	adjacent parent = current nodepos
					adjacentNode.parent = currentPosition;
					//	adjacent f = adjacent g + adjacent heuristic
					adjacentNode.f = adjacentNode.g + adjacentNode.h;
				}
				//	End
				//	Else If adjacent node is unused
				else if (adjacentNode.state == Node::StateNone)
				{
					//	adjacent G = new G
					adjacentNode.g = newG;
					//	adjacent h = heuristic
					adjacentNode.h = 0;
					//	adjacent parent = current nodepos
					adjacentNode.parent = currentPosition;
					//	adjacent f = adjacent g + adjacent heuristic
					adjacentNode.f = adjacentNode.g + adjacentNode.h;
					//	set adjacent node to open and push on the open list
					adjacentNode.state = Node::StateOpen;
					openList.push_back(adjacentPosition);
				}
				//	End
			}
			//	End
		}
		//	End
		//	Remove current node from open list and change it's state to closed.
		currentNode.state = Node::StateClosed;
		for (auto it = openList.begin(); it != openList.end(); it++)
		{
			//	If adjacent node is target node then pathFound = true
			if (*it == currentPosition)
			{
				openList.erase(it);
				break;
			}
			//	End
		}



		//	Now you can follow parent nodes from target back until the start to get the path.

		//	To fill the map(dijkstra), set adjacent h to 0 above.
		//	This pseudo code doesn't stop when it finds a path, it keeps filling. To make it stop early, change the while loop to be while(openList is not empty and pathFound is false)
	}


	


}

void Blank::setPosition()

{
	int index;
	do
	{
		target.set(int(m_rand.norm() * (m_initialData.mapData.width - 3)) + 1.5, int(m_rand.norm() * (m_initialData.mapData.height - 3)) + 1.5);
		index = int(target.x) + int(target.y) * m_initialData.mapData.width;
	} while (m_initialData.mapData.data[index].wall);
}

void Blank::result(bool won)
{
}

void Blank::bulletResult(bool hit)
{

}
