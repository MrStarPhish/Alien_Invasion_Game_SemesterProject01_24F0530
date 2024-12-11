//#include<iostream>
//using namespace std;
//
//int laser_count = 5;
//int laser_y[20] = { 0,1, 4, 5 ,6 };
//int laser_x[20] = { 6,6, 6, 6 ,6 };
//
//void swapArrElem(int a, int b) {
//	int temp = 0;
//	temp = laser_y[a];
//	laser_y[a] = laser_y[b];
//	laser_y[b] = temp;
//}
//
//int main()
//{
//	
//	for (int i = 0; i < laser_count; i++)
//	{
//		cout << laser_y[i] << "  ";
//	}
//	cout << endl;
//
//	for (int j = 0; j < laser_count; j++) // Trying to bubble sort
//	{
//		for (int k = 0; k < laser_count - 1; k++)
//		{
//			if(laser_y[k] == 0)
//				swapArrElem(k, k + 1);
//		}
//	}
//
//	laser_count--;
//
//	for (int i = 0; i < laser_count; i++)
//	{
//		cout << laser_y[i] << "  ";
//	}
//	cout << endl;
//
//	system("pause");
//	return 0;
//}
//

//void sortAsteroid()
//{
	//for (int j = 0; j < ast_count; j++) // Re-arrange the asteroid arrays
	//{
	//	for (int k = 0; k < ast_count; k++)
	//	{
	//		if (ast_y[k] == 49)
	//		{
	//			int temp = 0;
	//			temp = ast_y[k];
	//			ast_y[k] = ast_y[k + 1];
	//			ast_y[k + 1] = temp;

	//			temp = 0; ast_x[k] = 0;
	//			temp = ast_x[k];
	//			ast_x[k] = ast_x[k + 1];
	//			ast_x[k + 1] = temp;
	//		}
	//	}
	//}
//}