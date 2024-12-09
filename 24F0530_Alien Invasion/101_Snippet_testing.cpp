//#include<iostream>
//#include<cstring>
//using namespace std;
//int main()
//{
//	int t;
//	int sum = 0;
//	string lines[10] = {};
//	int charCount[10] = {};
//	int count = 0;
//	int streak = 0;
//	int j = 0;
//	bool input_error = 0;
//	bool validString = 0;
//	
//
//	cin >> t;  // inputting t
//	if (!(t>0 && t<11)) 
//	{
//		cout << "Invalid Input";
//		return 0;
//	}
//	//t = 2;
//	/*for (int i = 1; i <= t; i++)
//	{
//		switch (i)
//		{
//		case 1:
//			cin >> s1;
//			break;
//		case 2:
//			cin >> s2;
//			break;
//
//		case 3:
//			cin >> s3;
//			break;
//
//		case 4:
//			cin >> s4;
//			break;
//
//		case 5:
//			cin >> s5;
//			break;
//
//		case 6:
//			cin >> s6;
//			break;
//
//		case 7:
//			cin >> s7;
//			break;
//
//		case 8:
//			cin >> s8;
//			break;
//		
//		case 9:
//			cin >> s9;
//			break;
//
//		case 10:
//			cin >> s10;
//			break;
//
//		}
//
//	}*/
//
//	for (int i = 0; i < t; i++) // Inputting t-lines
//	{
//		validString = 1;
//		cin >> lines[i];
//		string temp = lines[i];
//		for (int j = 0; temp[j]!='\0'; j++)
//		{
//			if (!(temp[j] >= 'a' && temp[j] <= 'z'))
//				validString = 0;
//		}
//		if (validString == 0)
//		{
//			cout << "Invalid String Entry. It must be (a-z)" << endl;
//			return 0;
//		}
//	}
//	
//	
//	
//
//	for (int z = 0; z < t; z++)
//	{
//		count = 0;
//		string temp = lines[z];
//		for (int i = 0; temp[i] != '\0'; i++)
//		{
//			count++;
//		}
//		charCount[z] = count;
//	}
//
//	for (int k = 0; k < charCount[k]; k++)  // // each string
//	{
//		string temp = lines[k];
//		sum = 0;
//		for (int v = 0; v < charCount[k]; v++) // each combo
//		{
//			j = 0;
//			count = 0;
//			
//			for (int i = v; temp[i] != '\0'; i++) // each letter
//			{
//				if (temp[i] == temp[j]) // if continuous matches
//				{
//					count++;
//					j++;
//
//				}
//
//				else
//				{
//					break;
//				}
//				streak = count;
//			}
//				sum = sum + count;
//			//cout << count;
//		}
//		cout  <<sum << endl;
//		
//	}
//	
//
//
//	system("pause");
//	return 0;
//}