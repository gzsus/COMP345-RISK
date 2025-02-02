//////////////////////////////////////////////////////////////////////////////////////////////////////
//
//								MapLoader.cpp Samuel Renaud
//
//
//
//		
//////////////////////////////////////////////////////////////////////////////////////////////////////


#include "MapLoader.h"
#include "Map.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>



////////////////////////////////-MAPLOADER PART-/////////////////////////////////////////////
std::string file;
//empty constructor
MapLoader::MapLoader()
{
	file = "";
}
//copy constructor
MapLoader::MapLoader(const MapLoader& a) {file = a.file;}

//constructor that takes in a file string and creates a map right away
MapLoader::MapLoader(std::string mapfile)
{
	FileReader(mapfile);
}

MapLoader::~MapLoader()
{
	std::cout << "Deleting MapLoader \n";
	delete created_map;
	created_map = NULL;
}

//instead of directly reding the file in the constructor now this method does the filereading
void MapLoader::FileReader(std::string mapfile)
{
	bool error = false; //if there is an error at one point this is set to true and the program breaks
	file = mapfile;
	//std::cout << mapfile;
	std::ifstream input(mapfile);
	std::string line;
	getline(input, line, '\n');
	std::cout << line;
	while (!input.eof())
	{
		//these are temporary
		std::string temp; //used to store each line from the file
		int index = 0; //used to get size of data or variable to store in vector
		int counter = 0; //keeps the count of the number of countries to determine number of borders

		////////////////////////////CONTINENTS FROM .MAP/////////////////////////////////////////////////
		getline(input, temp, '\n');
		std::cout << temp << "\n";
		if (temp.find("[continents]") != -1) //store all the continents
		{
			counter = 0;  //will allow storage location management for 2d-vector
			while (temp.find("[countries]") == -1)
			{
				continent_list.push_back(std::vector<std::string>()); //allocate new space in top vector
				getline(input, temp, '\n');
				for (int i = 0; i < 3; i++)
				{
					if (index == -1 && i != 0) //missing values
					{
						index = -3;
						break;
					}
					index = temp.find(" "); // store the length of the name of the continent in index
					continent_list[counter].push_back(temp.substr(0, index));
					temp.erase(0, index + 1);
					if (index == -1 && i != 0) { continue; } //no color for continent
					if (index == -1) // no more continents to store
					{
						index = -2;
						break;
					}
				}
				if (index == -2 || index == -3)
				{
					continent_list.erase(continent_list.begin() + (continent_list.size() - 1)); //gets rid of extra country added
					break;
				}
				std::cout << "VECTOR continent #" << counter << " :" << continent_list[counter][0] << " " << continent_list[counter][1] << " " << continent_list[counter][2] << "\n";
				counter++;
			}
		}
		counter = 0; //reset for use in other file reading code

		////////////////////////////COUNTRIES FROM .MAP/////////////////////////////////////////////////
		if (temp.find("[countries]") != -1)
		{
			while (temp.find("[borders]") == -1)
			{
				getline(input, temp, '\n');
				if (temp.find(';') != -1) { continue; }//found a map that has comments after the countries declaration...
				country_list.push_back(std::vector<std::string>()); //allocate vector space for top vector
				for (int i = 0; i < 5; i++)
				{
					if (index == -1 && i != 0) //one of the inputs is missing! 
					{
						index = -3; //use the index as a error indicator -3 means country formatting was problematic
						break;
					}
					index = temp.find(" "); // store the index of the current variable to be stored

					if (i == 0 && index == -1) //this is an empty line and we are done with this section
					{
						index = -2; //needed a way to break out the other loop too 
						break;
					}
					country_list[counter / 5].push_back(temp.substr(0, index));
					temp.erase(0, index + 1);
					counter++;
				}
				if (index == -2 || index == -3) {
					country_list.erase(country_list.begin() + (country_list.size() - 1)); //gets rid of extra country added
					break;
				} //we are done so break out of this too
				std::cout << "VECTOR country #" << (counter / 5) - 1 << " :" << country_list[(counter / 5) - 1][0] << " " << country_list[(counter / 5) - 1][1] << " " << country_list[(counter / 5) - 1][2] << " " << country_list[(counter / 5) - 1][3] << " " << country_list[(counter / 5) - 1][4] << "\n";
			}

		}

		////////////////////////////ERROR CHECKING/////////////////////////////////////////////////
		//check continent count and parameters ERROR CHECKING
		//to check continents you need to read all countries and make sure that the 3rd element in the index, the continent to which the country belongs
		//adds up properly to the number of continents read
		if (index == -3)
		{
			std::cout << " error in .map file";
			error = true;
			break;
		}
		if (continent_list.size() == 0 && country_list.size() != 0 || continent_list.size() != 0 && country_list.size() == 0)
		{
			if (continent_list.size() == 0)
			{
				std::cout << "no continents stored, error in .map file";
				error = true;
				break;
			}
		}
		if (continent_list.size() != 0 && country_list.size() != 0 && border_list.size() == 0)
		{
			std::vector<int> number_of_countries;
			for (int i = 0; i < country_list.size(); i++) { number_of_countries.push_back(stoi(country_list[i][2])); }
			auto maximum = std::max_element(number_of_countries.begin(), number_of_countries.end());
			if (continent_list.size() == (*maximum))
			{
				std::cout << "continent number has been confirmed with countries at:" << continent_list.size() << " continents" << "\n";
			}
			else
			{
				std::cout << "number of continents and countries do not match !";
				std::cout << "number of continents: " << continent_list.size() << " number of countries: " << (*maximum);
				error = true;
				break;
			}
		}

		////////////////////////////BORDERS FROM .MAP/////////////////////////////////////////////////
		if (temp.find("[borders]") != -1)
		{
			for (int j = 0; j < (country_list.size()); j++) //go through the "j" number of countries and document border info
			{
				getline(input, temp, '\n');
				if (temp.rfind(" ") >= temp.size() - 1) { temp = temp.substr(0, temp.size() - 1); }
				if (temp.empty() == true) //not enough borders given
				{
					std::cout << "Not enough borders given!";
					error = true;
					break;
				}
				if (temp.at(0) == ';') //found a map that has comments after the continent declaration...
				{
					j = j - 1;
					continue;
				}
				border_list.push_back(std::vector<std::string>()); //allocate vector space for top vector
				do //in order to get the last value save first then check condition
				{
					index = temp.find(" "); // store the index of the current variable to be stored
					border_list[j].push_back(temp.substr(0, index)); //store into border item list
					temp.erase(0, index + 1);
				} while (index != -1);
				for (int k = 0; k < border_list[j].size(); k++)
				{
					std::cout << border_list[j][k] << " ";
				}
				std::cout << "\n";
			}
			getline(input, temp, '\n');
			if (temp.empty() == false) //make sure the last border line was read
			{
				std::cout << "error in reading file more borders than countries given";
				error = true;
				break;
			}
		}

	}
	input.close();

	std::cout << "\n";

	if (error == false) //if there were no errors in reading the file build the map
	{
		//now build the map using the extracted information and store it into a pointer
		created_map = new Map(LoadMap(continent_list, country_list, border_list));
	}

	else
	{
		//adressing memory leak
		continent_list.clear();
		country_list.clear();
		border_list.clear();
	}
}
// this will load the map from the stored vectors 
 Map MapLoader::LoadMap(std::vector<std::vector<std::string>>continents, std::vector<std::vector<std::string>> territories, std::vector<std::vector<std::string>>borders)
{
	 //create a new *territory list for each index in the territories vector and store the name
	 std::vector<Territory*> territory_list;
	 std::vector<Continent*> cont_list;
	 for (int i = 0; i < territories.size();i++) 
		{ territory_list.push_back(new Territory(territories[i][1], 0));}

	 //need to connect all the territories now using the border list
	 for (int i = 0; i < territories.size(); i++)
	 {
		 //need to loop through all border indices to get all neighbours, annoyingly the first index is just that of the same country so reduce accordingly
		 for (int j = 1; j < borders[i].size(); j++)
			{territory_list[i]->connet_to(territory_list[stoi(borders[i][j])-1]);}
	 }

	 //now that all the territories are created and linked create continents and link them to countries
	 for (int i = 0; i < continents.size(); i++)
	 {
		 //this should also implement bonus but for now it doesnt cause the class is not available
		 cont_list.push_back(new Continent(continents[i][0]));
	 }

	 //go through continents
	 //int j=0; // once we complete a series of countries counted by j we dont want j=0 again it should just keep going from where it was before
	 for (int i = 0; i < cont_list.size(); i++)
	 {
		 for (int j = 0; j < territories.size(); j++)
		 {
			 if (i + 1 == stoi(territories[j][2]))
			 { cont_list[i]->add_territory(territory_list[j]);}
		 }
	 }

	 //Finally create a new map and add the continents to it
	 Map* created_map = new Map("created_map");
	 for (int i = 0; i < cont_list.size(); i++)
	 { created_map->add_continent(cont_list[i]);}

	 if (false == created_map->validate())
	 { std::cout << "validation of the map has failed! \n";}
	 else
	 { std::cout << "map validation succesful! \n";}


	 return *created_map;
}

//assingment operator
 MapLoader& MapLoader::operator=(const MapLoader& oldloader)
 {
	 file = oldloader.file;
	 return *this;
 }

 
 //set the file for the maploader object
void MapLoader::setfile(std::string newfile) {file = newfile;}
//get the file for this object
std::string MapLoader::getfile(){return file;}
//tostring overload
std::ostream& operator<<(std::ostream &ostrm, const MapLoader &maploader)
{	return ostrm << "Maploader Loading: (" << maploader.file << ")"; }

////////////////////////////////-CONQUEST FILE READER PART-/////////////////////////////////////////////
//implement a file reader instead of an all in one constructor for both the maploader and for the conquest file reader
ConquestFileReader::ConquestFileReader()
{
	file = "";
	created_map = new Map();
}

ConquestFileReader::ConquestFileReader(const ConquestFileReader& a)
{
	file = a.file;
	created_map = a.created_map;
}

ConquestFileReader::ConquestFileReader(std::string mapfile)
{
	FileReader(mapfile);
}

void ConquestFileReader::FileReader(std::string mapfile)
{
	////////////////////////////////-ACTUALLY IMPLEMENT THE FILE READER-/////////////////////////////////////////////
	file = mapfile;
	//std::cout << mapfile;
	std::ifstream input(mapfile);
	std::string line;
	int counter=0;
	while (!input.eof())
	{
		getline(input, line, '\n');
		///////////////////-CONTINENTS-/////////////////
		if (line.compare("[Continents]")==0)
		{
			while (!input.eof())
			{
				getline(input, line, '\n');
				if (line.compare("[Territories]")==0) { break; }
				if (line.empty()) { continue; }

				continent_list.push_back(std::vector<std::string>());
				continent_list[counter].push_back(line.substr(0,line.find('=')));	//store the continent name
				continent_list[counter].push_back(line.substr(line.find('=')+1)); //add 1 to not include the equals sign when storing associated points
				std::cout <<"Continent:" <<continent_list[counter][0]<<"\t Number:"<< continent_list[counter][1] <<"\n";
				counter++;
			}
		}
		counter = 0;
		///////////////////-TERRITORIES/COUNTRIES & BORDERS-/////////////////
		///////////////////In order for the loadmap function to be compatible with this new format some re-formatting will be done/////////////////
		/////////////////notably the first 4 comma delimited .map file strings are the only info in the territories vector ///////////////////
		/////////////////of these the last one should be in the first second index position after the name which itself is after the country count number ///////////////////
		/////////////////finally the border substring will be processsed according to the old format ///////////////////
		if (line.compare("[Territories]") == 0)
		{
			while (!input.eof())
			{
				getline(input, line, '\n');
				if (line.empty()) { continue; }

				country_list.push_back(std::vector<std::string>());
				country_list[counter].push_back(std::to_string(counter));
				for (int i = 0; i < 4; i++)
				{
					country_list[counter].push_back(line.substr(0, line.find(',')));
					line.erase(0, line.find(',') + 1);	//+1 also deletes the comma
				}
				std::swap(country_list[counter][2], country_list[counter][4]);
				std::swap(country_list[counter][3], country_list[counter][4]);
				std::cout <<"Country: "<< country_list[counter][0] << " " << country_list[counter][1] << " " << country_list[counter][2] << " " << country_list[counter][3] <<" "<< country_list[counter][4] << "\n";
				
				
				///////////////////-BORDERS-/////////////////
				border_list.push_back(std::vector<std::string>());
				while (line.empty() == false)
				{
					if (line.find(',') == -1) { break; }
					border_list[counter].push_back(line.substr(0, line.find(',')));
					line.erase(0, line.find(',') + 1);	//+1 also deletes the comma
				}
				std::cout << "Borders: ";
				for (int i = 0; i < border_list[counter].size(); i++) { std::cout << border_list[counter][i]<<"\t"; }
				counter++;
				std::cout << "\n";
			}
		}
	}


	///////////////////-CONTINENT REFERNCING BY NAME CHANGED TO REFERENCE BY INDEX-/////////////////
	for (int i = 0; i < country_list.size(); i++)
	{
		for (int j = 0; j < continent_list.size(); j++)
		{
			if (country_list[i][2].compare(continent_list[j][0])==0)
			{
				country_list[i][2] = std::to_string(j);
			}
		}
	}

	///////////////////-BORDER REFERENCE BY NAME CHANGED TO BY INDEX-/////////////////
	for (int i = 0; i < border_list.size(); i++)
	{
		for (int j = 0; j < border_list[i].size(); j++)
		{
			for (int k = 0; k < country_list.size(); k++)
			{
				if (border_list[i][j].compare(country_list[k][1])==0)
				{
					border_list[i][j] = country_list[k][0];
					break;
				}	
			}
		}
	}

	std::cout << "\n";
	std::cout << "Border in number format\n";
	for (int i = 0; i < border_list.size(); i++)
	{
		std::cout << "Borders: ";
		for (int j = 0; j < border_list[i].size(); j++)
		{
			 std::cout << border_list[i][j] << "\t"; 
		}
		std::cout << "\n";
	}

	created_map = new Map(LoadMap(continent_list, country_list, border_list));


	//adressing memory leak
	continent_list.clear();
	country_list.clear();
	border_list.clear();
	return;
}

Map ConquestFileReader::LoadMap(std::vector<std::vector<std::string>> continents, std::vector<std::vector<std::string>> territories, std::vector<std::vector<std::string>> borders)
{
	//create a new *territory list for each index in the territories vector and store the name
	std::vector<Territory*> territory_list;
	std::vector<Continent*> cont_list;

	for (int i = 0; i < territories.size(); i++)
	{territory_list.push_back(new Territory(territories[i][1], 0));}

	//need to connect all the territories now using the border list
	for (int i = 0; i < territories.size(); i++)
	{
		//need to loop through all border indices to get all neighbours, annoyingly the first index is just that of the same country so reduce accordingly
		for (int j = 0; j < borders[i].size(); j++)
		{
			territory_list[i]->connet_to(territory_list[stoi(borders[i][j])]);
		}
	}

	//now that all the territories are created and linked create continents and link them to countries
	for (int i = 0; i < continents.size(); i++)
	{
		//this should also implement bonus but for now it doesnt cause the class is not available
		cont_list.push_back(new Continent(continents[i][0]));
	}

	//go through continents
	//int j=0; // once we complete a series of countries counted by j we dont want j=0 again it should just keep going from where it was before
	for (int i = 0; i < cont_list.size(); i++)
	{
		for (int j = 0; j < territories.size(); j++)
		{
			if (i == stoi(territories[j][2]))
			{cont_list[i]->add_territory(territory_list[j]);}
		}
	}

	//Finally create a new map and add the continents to it
	Map* created_map = new Map("created_map");
	for (int i = 0; i < cont_list.size(); i++)
	{created_map->add_continent(cont_list[i]);}

	if (false == created_map->validate())
	{std::cout << "validation of the map has failed! \n";}

	else
	{std::cout << "map validation succesful! \n";}

	return *created_map;
}

ConquestFileReader& ConquestFileReader::operator=(const ConquestFileReader& oldloader)
{
	file = oldloader.file;
	created_map = oldloader.created_map;
	return  *this;
}

ConquestFileReader::~ConquestFileReader()
{
	std::cout << "Deleting Conquest Maploader \n";
	delete created_map;
	created_map = NULL;
}




ConquestFileReaderAdapter::ConquestFileReaderAdapter(ConquestFileReader conquest_file_reader)
{
	conquest_map = new ConquestFileReader();
	*conquest_map = conquest_file_reader;
}

void ConquestFileReaderAdapter::FileReader(std::string mapfile)
{
	std::cout << "--Using the adapter to create a Conquest map!-- \n";
	conquest_map->FileReader(mapfile);
}

