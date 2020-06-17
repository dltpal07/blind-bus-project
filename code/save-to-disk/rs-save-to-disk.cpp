

// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2015-2017 Intel Corporation. All Rights Reserved.
#define _CRT_SECURE_NO_WARNINGS
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <Windows.h>
#include <fstream>              // File IO
#include <iostream>             // Terminal IO
#include <sstream>              // Stringstreams
#include <vector>
#include <string>
#include <cstring>

#include <sapi.h>
#include <stdlib.h> 
#include <locale.h> 
// 3rd party header for writing png files
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "pythonInclude.h"

using namespace std;
// Helper function for writing metadata to disk as a csv file
void metadata_to_csv(const rs2::frame& frm, const std::string& filename);

// This sample captures 30 frames and writes the last frame to disk.
// It can be useful for debugging an embedded system with no display.
int main(int argc, char * argv[]) try
{
	
	int i = 0;
	// Declare depth colorizer for pretty visualization of depth data
	PyObject *pName, *pModule, *pFunc, *pValue;
	Py_Initialize();
	HRESULT hr;
	hr = ::CoInitialize(NULL);

	/*PyObject* myModuleString = PyUnicode_FromString((char*)"hi");
	PyObject* myModule = PyImport_Import(myModuleString);*/

	//////////////////////////////웹에서 입력한 버스번호 받아오기//////////////////////////
	
	string base;

	if (strcmp(argv[1], "bnum") == 0)
	{
		base = argv[2];
		cout << "> we will find bus number " << base << endl;
	}
	
	else
	{
		cout << "> we will find " << argv[1] << endl;
	}

	while (1)
	{
		cout << "\n\n ---------- ready for taking picture ---------- \n";
		////////////////////////////////카메라찍기/////////////////////////////////////////
		rs2::colorizer color_map;

		// Declare RealSense pipeline, encapsulating the actual device and sensors
		rs2::pipeline pipe;
		// Start streaming with default recommended configuration

		pipe.start();

		// Capture 30 frames to give autoexposure, etc. a chance to settle
		for (auto i = 0; i < 30; ++i) pipe.wait_for_frames();

		// Wait for the next set of frames from the camera. Now that autoexposure, etc.
		// has settled, we will write these to disk
		rs2::frameset frames = pipe.wait_for_frames();
		rs2::depth_frame depth = frames.get_depth_frame();
		cout << "\n";
		for (auto&& frame : pipe.wait_for_frames())
		{

			// We can only save video frames as pngs, so we skip the rest
			if (auto vf = frame.as<rs2::video_frame>())
			{
				auto stream = frame.get_profile().stream_type();
				// Use the colorizer to get an rgb image for the depth stream
				if (vf.is<rs2::depth_frame>()) vf = color_map.process(frame);

				// Write images to disk
				std::stringstream png_file;
				//png_file << "image"<<i << vf.get_profile().stream_name() << ".png";
				png_file << "image" << vf.get_profile().stream_name() << 0 << ".png";
				stbi_write_png(png_file.str().c_str(), vf.get_width(), vf.get_height(),
					vf.get_bytes_per_pixel(), vf.get_data(), vf.get_stride_in_bytes());
				
				std::cout << "> Saved Picture" << 0 << png_file.str() << std::endl;
				
				// Record per-frame metadata for UVC streams
				//std::stringstream csv_file;
				//csv_file << "rs-save-to-disk-output-"<<i << vf.get_profile().stream_name()
				//<< "-metadata.csv";
				//metadata_to_csv(vf, csv_file.str());
			}
		}
		cout << "\n";

		// 좌표값을 받은 다음에, frame에 대해서 depth부분에 좌표값을 넣어 depth를 추출한다.



		/////////////////////////////객체인식 돌리기/////////////////////////////////////
		PyObject* myModuleString = PyUnicode_FromString((char*)"hi");
		PyObject* myModule = PyImport_Import(myModuleString);
		PyObject* myFunction = PyObject_GetAttrString(myModule, (char*)"st");
		PyObject *pArg = Py_BuildValue("(z)", argv[1]);
		//PyObject *pArg = Py_BuildValue("(z)", (char *)"bnum");
		//PyObject *pArg = Py_BuildValue("(z)", (char *)"bell");
		//pValue = PyObject_CallObject(myFunction, NULL);
		//cout << "find object is " << argv[1]<<endl;
		pValue = PyObject_CallObject(myFunction, pArg);

		if (pValue != NULL)
		{
			//printf("%d\n", PyList_Check(pValue));
			int count = (int)PyList_Size(pValue);
			//printf("count : %d\n", count);
			vector<int> temp(count);
			string str1;
			PyObject *ptemp, *objectsRepresentation;
			const char *all;
			float dist_to_center = 0;
			for (int i = 0; i < count; i++)
			{

				ptemp = PyList_GetItem(pValue, i);
				objectsRepresentation = PyObject_Repr(ptemp);
				PyObject* str = PyUnicode_AsEncodedString(objectsRepresentation, "utf-8", "~E~");
				all = PyBytes_AsString(str);
				if (i < count - 1)
				{

					temp[i] = (int)strtod(all, NULL);
				}
				else {
					str1 = all;
				}

			}

			//////////////////////////숫자인식 돌리기/////////////////////////////////
			string result;
			
			if (strcmp(argv[1], "bnum") == 0 && str1 == "\'bnum\'")
			{

				PyObject* myModuleString_1 = PyUnicode_FromString((char*)"tmp");
				PyObject* myModule_1 = PyImport_Import(myModuleString_1);
				PyObject* myFunction_1 = PyObject_GetAttrString(myModule_1, (char*)"get_int");
				PyObject *pArg_1 = Py_BuildValue("(i, i, i, i)", temp[0], temp[1], temp[2], temp[3]);
				PyObject *pValue_1 = PyObject_CallObject(myFunction_1, pArg_1);

				if (pValue == NULL)
				{
					cout << "> Return value is NULL";
				}

				PyObject *objectsRepresentation = PyObject_Repr(pValue_1);
				PyObject* str = PyUnicode_AsEncodedString(objectsRepresentation, "utf-8", "~E~");
				string result = PyBytes_AsString(str);
				result = result.substr(1, result.size() - 2);
				
				cout << "> we want to find bus number " << base << ", and we find bus number" << result << "\n";

				////////////////////////////입력한 버스 번호와 일치//////////////////////////////////////
				if (base.compare(result) == 0)
				{
					cout << "> we find bus we want\n";
					dist_to_center = depth.get_distance((temp[1] + temp[3]) / 2, (temp[0] + temp[1]) / 2);
					std::cout << "> The camera is facing an " << str1 << " is " << dist_to_center << " meters away \n";
					
					Sleep(10000);
					//////////////////////////////음성출력////////////////////////////////////
					//name 번이 dist_to_center 앞에 있습니다.
					
					ISpVoice * pVoice = NULL;
					char name[100];
					char dtc_cha[10];
					int idx = result.find("-");
					if (idx != -1) 
					{
						int len = result.length() - idx;
						string result1 = result.substr(0, idx);
						string result2 = result.substr(idx + 1, len);
						result1.push_back(' ');
						result1.push_back('-');
						result1.push_back(' ');
						for (int i = 0; i < result2.length(); i++)
							result1.push_back(result2[i]);
						result = result1;
					}

					strcpy(name, result.c_str());
					strcat(name, "버니 ");
					sprintf(dtc_cha, "%.1f", dist_to_center);
					strcat(name, dtc_cha);
					strcat(name, "미터 앞에 있습니다\n");
					std::cout << name;
					Sleep(10000);
					wchar_t text1[100];

					setlocale(LC_ALL, "");
					mbstowcs(text1, name, strlen(name) + 1);


					hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
					if (SUCCEEDED(hr))
					{
						hr = pVoice->Speak(text1, 0, NULL);
						pVoice->Release();
						pVoice = NULL;
					}
					
					::CoUninitialize();
					Sleep(10000);
					Py_Finalize();
					//음성출력끝
					break;
				}
			}
			else if (strcmp(argv[1], "bell") == 0 && str1 == "\'bell\'")
			{
				cout << "> we find bus bell\n";
				dist_to_center = depth.get_distance((temp[1] + temp[3]) / 2, (temp[0] + temp[1]) / 2);
				std::cout << "> The camera is facing an " << str1 << " is " << dist_to_center << " meters away \n";

				//////////////////////////////음성출력////////////////////////////////////
				//하차벨이 dist_to_center 미터 앞에 있습니다.
				ISpVoice * pVoice = NULL;
				char name[100] = {};
				char dtc_cha[10];

				strcat(name, "하차벨이 ");
				sprintf(dtc_cha, "%.1f", dist_to_center);
				strcat(name, dtc_cha);
				strcat(name, "미터 앞에 있습니다");
				std::cout << name;

				wchar_t text1[100];

				setlocale(LC_ALL, "");
				mbstowcs(text1, name, strlen(name) + 1);

				hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
				if (SUCCEEDED(hr))
				{
					hr = pVoice->Speak(text1, 0, NULL);
					pVoice->Release();
					pVoice = NULL;
				}
				::CoUninitialize();
				//음성출력끝

				Py_Finalize();
				break;
			}

			else if (strcmp(argv[1], "entrance") == 0 && str1 == "\'entrance\'")
			{
				cout << "> we find entrance\n";
				dist_to_center = depth.get_distance((temp[1] + temp[3]) / 2, (temp[0] + temp[1]) / 2);
				std::cout << "> The camera is facing an " << str1 << " is " << dist_to_center << " meters away \n";

				//////////////////////////////음성출력////////////////////////////////////
				//입구가 dist_to_center 미터 앞에 있습니다.
				ISpVoice * pVoice = NULL;
				char name[100] = {};
				char dtc_cha[10];

				strcat(name, "입구가 ");
				sprintf(dtc_cha, "%.1f", dist_to_center);
				strcat(name, dtc_cha);
				strcat(name, "미터 앞에 있습니다");
				std::cout << name;

				wchar_t text1[100];

				setlocale(LC_ALL, "");
				mbstowcs(text1, name, strlen(name) + 1);


				hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
				if (SUCCEEDED(hr))
				{
					hr = pVoice->Speak(text1, 0, NULL);
					pVoice->Release();
					pVoice = NULL;
				}
				::CoUninitialize();
				//음성출력끝

				Py_Finalize();
				break;
			}
			else if (strcmp(argv[1], "seat") == 0 && str1 == "\'seat\'")
			{
				cout << "> we find empty seat\n";
				dist_to_center = depth.get_distance((temp[1] + temp[3]) / 2, (temp[0] + temp[1]) / 2);
				std::cout << "> The camera is facing an " << str1 << " is " << dist_to_center << " meters away \n";

				//////////////////////////////음성출력////////////////////////////////////
				//빈 좌석이 dist_to_center 미터 앞에 있습니다.
				ISpVoice * pVoice = NULL;
				char name[100] = {};
				char dtc_cha[10];

				strcat(name, "빈 좌석이 ");
				sprintf(dtc_cha, "%.1f", dist_to_center);
				strcat(name, dtc_cha);
				strcat(name, "미터 앞에 있습니다");
				std::cout << name;

				wchar_t text1[100];

				setlocale(LC_ALL, "");
				mbstowcs(text1, name, strlen(name) + 1);


				hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
				if (SUCCEEDED(hr))
				{
					hr = pVoice->Speak(text1, 0, NULL);
					pVoice->Release();
					pVoice = NULL;
				}
				::CoUninitialize();
				//음성출력끝

				Py_Finalize();
				break;
			}
			else if (strcmp(argv[1], "terminal") == 0 && str1 == "\'terminal\'")
			{
				cout << "> we find terminal\n";
				dist_to_center = depth.get_distance((temp[1] + temp[3]) / 2, (temp[0] + temp[1]) / 2);
				std::cout << "> The camera is facing an " << str1 << " is " << dist_to_center << " meters away \n";

				//////////////////////////////음성출력////////////////////////////////////
				//단말기가 dist_to_center 미터 앞에 있습니다.
				ISpVoice * pVoice = NULL;
				char name[100] = {};
				char dtc_cha[10];

				strcat(name, "단말기가 ");
				sprintf(dtc_cha, "%.1f", dist_to_center);
				strcat(name, dtc_cha);
				strcat(name, "미터 앞에 있습니다");
				std::cout << name;

				wchar_t text1[100];

				setlocale(LC_ALL, "");
				mbstowcs(text1, name, strlen(name) + 1);

				hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
				if (SUCCEEDED(hr))
				{
					hr = pVoice->Speak(text1, 0, NULL);
					pVoice->Release();
					pVoice = NULL;
				}
				::CoUninitialize();
				//음성출력끝

				Py_Finalize();
				break;
			}

			else
			{
			//////////////////////////////음성출력////////////////////////////////////
				//원하는 물체를 찾을 수 없습니다. 고개를 다른 곳으로 돌려주세요.
				ISpVoice * pVoice = NULL;
				char name[100] = {"원하는 물체를 찾을 수 없습니다. 고개를 다른 곳으로 돌려주세요"};
				char dtc_cha[10];

				

				wchar_t text1[100];

				setlocale(LC_ALL, "");
				mbstowcs(text1, name, strlen(name) + 1);


				hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice);
				if (SUCCEEDED(hr))
				{
					hr = pVoice->Speak(text1, 0, NULL);
					pVoice->Release();
					pVoice = NULL;
				}
				::CoUninitialize();
				//음성출력끝

 
			}
			Sleep(10000);


		}
		else
		{
			printf("> pValue is Null\n");

		}



	}

	return EXIT_SUCCESS;
}
catch (const rs2::error & e)
{
	std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const std::exception & e)
{
	std::cerr << e.what() << std::endl;
	return EXIT_FAILURE;
}

void metadata_to_csv(const rs2::frame& frm, const std::string& filename)
{
	std::ofstream csv;

	csv.open(filename);

	//    std::cout << "Writing metadata to " << filename << endl;
	csv << "Stream," << rs2_stream_to_string(frm.get_profile().stream_type()) << "\nMetadata Attribute,Value\n";

	// Record all the available metadata attributes
	for (size_t i = 0; i < RS2_FRAME_METADATA_COUNT; i++)
	{
		if (frm.supports_frame_metadata((rs2_frame_metadata_value)i))
		{
			csv << rs2_frame_metadata_to_string((rs2_frame_metadata_value)i) << ","
				<< frm.get_frame_metadata((rs2_frame_metadata_value)i) << "\n";
		}
	}

	csv.close();
}
