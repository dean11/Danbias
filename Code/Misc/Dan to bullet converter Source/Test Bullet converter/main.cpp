#include <iostream>
#include <string>
using namespace std;

#include "GraphicalDefinition.h"
#include "Bullet Source\btBulletDynamicsCommon.h"
#include "Bullet Source\BulletWorldImporter\btBulletWorldImporter.h"
#include "Bullet Source\BulletFileLoader\btBulletFile.h"

ModelInfo LoadDAN(const char filename[]);
void UnloadDAN(ModelInfo& info);

/*
	This project has not been checked for memory leaks 
	and may crash at any time.
*/

int main(int argc, const char* argv[])
{
	if( argc < 2)
	{
		printf("To few arguments! Need a filename at least\n");
		system("pause");
		return 0;
	}
	std::string filename = "";
	if(argc > 1)
		filename = argv[1];

	std::string savePath = "";
	if(argc > 2)
		savePath = argv[2];

	cout << "Convert your .dan files to .bullet collision files." << endl;
	cout << "The file will be saved as your input filename + '.bullet'." << endl;

	//while(1)
	//{
		//cout << "\nInput filename: ";
		//getline(cin, filename);

		ModelInfo model;
		model = LoadDAN(filename.c_str());

		if(model.numIndex > 0 && model.numVertex > 0)
		{
			btScalar* vertices = new btScalar[model.numVertex*3];
			for(int i = 0; i < (int)model.numVertex; i++)
			{
				vertices[i*3] = model.vertices[i].pos[0];
				vertices[(i*3)+1] = model.vertices[i].pos[1];
				vertices[(i*3)+2] = model.vertices[i].pos[2];
			}

			btTriangleIndexVertexArray* vertexArray = new btTriangleIndexVertexArray(model.numIndex/3, (int*)model.indices, 3*sizeof(float), model.numVertex, vertices, 3*sizeof(float));
			btBvhTriangleMeshShape* shape = new btBvhTriangleMeshShape(vertexArray, true);

			btDefaultSerializer* serializer = new btDefaultSerializer(1024*1024*5);

			serializer->startSerialization();

			shape->serializeSingleShape(serializer);

			serializer->finishSerialization();

			std::string outFilename = filename;

			// Remove file extension
			//outFilename = filename.replace( filename.find(".dan"), 4, ".bullet");
			outFilename.replace( outFilename.find(".dan"), 4, ".bullet");

			if(savePath.size() != 0)
				outFilename = savePath;

			//Print out the new file
			FILE* file;
			if(!fopen_s(&file, outFilename.c_str(), "wb"))
			{
				fwrite(serializer->getBufferPointer(), serializer->getCurrentBufferSize(), 1, file);
				fclose(file);

				cout << outFilename << " has been created" << endl;
			}
			else
			{
				cout << "Could not write to file: " << outFilename << endl;
			}
		

			UnloadDAN(model);
		}
		else
		{
			cout << "Could not load file" << endl;
		}
	//}

	/* Prints all vertices
	for(int i = 0; i < model.numVertex; i++)
	{
		for(int j = 0; j < 3; j++)
			cout << model.vertices[i].pos[j] << ' ';

		cout << endl;
	}*/
	system("pause");
	return 0;
}