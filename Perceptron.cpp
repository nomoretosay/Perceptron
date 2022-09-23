/*
* This algorithm is for Perceptron algorithm,now,let me show you some details
* before using this algorithm,please tag all the data's true or false
* what i mean in last sentence is that:specify whether the data is positive or negative
* in this way,if this datas is Linearly separable,i can seperate those datas by a hyperplane
* if those datas are not Linearly separable,i will return it false and stop this program.
* 
* 下面是中文翻译：
* 这个算法是感知机算法，现在，让我给你看一些细节
* 在使用此算法之前，请标记所有数据的正负
* 我上一句的意思是：指定数据是正数还是负数(也就是说他应该被分在超平面的哪一侧)
* 这样，如果这些数据是线性可分的，我可以用超平面分离这些数据
* 如果这些数据不是线性可分的，我将返回 false 并停止该程序。
*/
#include <iostream>
#include <fstream>
#include <string>//如果不加上这个头文件的话就会导致getline方法报错
#include <vector>
using namespace std;


//函数定义：
vector<vector<double>> getDatas();
vector<double> splitStr(string str);
pair<vector<double>, bool> isItClassifiedCorrectly(vector<vector<double>> datas,vector<double> w0,int b);
vector<double> matrixAdd(vector<double> data, vector<double> gradient);//第一个参数是w0的数组，第二个参数是w0的梯度
vector<double> matrixMultiplicationConstant(vector<double> matrix1, double y);//定义矩阵与常数的乘法
double matrixMultiplicationMatrix(vector<double> matrix1, vector<double> matrix2);//矩阵 × 矩阵


//第一步，进行数据读取和处理，请把所有的数据全部按照我上面所说放到一个文本中去，这样方便我们读取
vector<vector<double>> getDatas()
{
	vector<vector<double>> res;
	ifstream file;
	file.open("data.txt", ios::in);
	if (!file.is_open())
	{
		cout << "文件读取失败" << endl;
		return { {} };
	}
	string strLine;
	while (getline(file,strLine))
	{
		res.push_back(splitStr(strLine));
	}
	return res;
}


//第二步：分离数据：
vector<double> splitStr(string str)
{
	vector<double> res;
	string temp = "";
	for (int i = 0; i <= str.size(); ++i)
	{
		if (str[i] == ' ' || str[i] == '\0')//这里我们指定分隔符为空格，然后用空格这个分割符来进行分割，如果之后用了其他的分隔符号，请修改
		{
			//这里之所以要写一下str[i] == '\0'这个条件就是为了保证最后一个元素也能检索进去。
			res.push_back(stod(temp));
			temp = "";
		}
		else
		{
			temp += str[i];
		}
	}
	return res;
}


//对矩阵进行加法的函数，方便之后对w0进行直接操作
vector<double> matrixAdd(vector<double> data, vector<double> gradient)
{
	for (int i = 0; i < data.size(); ++i)
	{
		if (data[i] + gradient[i] > INT32_MAX) {
			cout << "矩阵×矩阵导致数的范围越界，请重新界定数的范围或重写程序，扩大数的范围" << endl; return {};
		}
		data[i] += gradient[i];
	}
	return data;
}


//矩阵 × 矩阵
double matrixMultiplicationMatrix(vector<double> matrix1, vector<double> matrix2)
{
	double res = 0.0;
	for (int i = 0; i < matrix1.size(); ++i)
	{
		if (matrix1[i] * matrix2[i] > INT32_MAX) {
			cout << "矩阵×矩阵导致数的范围越界，请重新界定数的范围或重写程序，扩大数的范围" << endl; return {};
		}
		res += matrix1[i] * matrix2[i];
	}
	return res;
}


//矩阵 × 常数
vector<double> matrixMultiplicationConstant(vector<double> matrix, double y)
{
	for (int i = 0; i < matrix.size(); ++i)
	{
		if (matrix[i] * y > INT32_MAX) {
			cout << "矩阵×常数导致数的范围越界，请重新界定数的范围或重写程序，扩大数的范围" << endl; return {};
		}
		matrix[i] *= y;
	}
	return matrix;
}


//第三步:拿到所有的数据，并且标记其分类正确与否，如果分类正确，返回空数组 + true，否则，返回这个分类错误的数据 + false
//该方法的第二个参数是所需要求的超平面w * x + b = 0的参数w和b所在的数组，w是前n个元素
pair<vector<double>, bool> isItClassifiedCorrectly(vector<vector<double>> datas,vector<double> w0,int b)
{
	for (int i = 0; i < datas.size(); ++i)
	{
		int n = datas[i].size() - 1;
		double yi = datas[i][n];//拿到最后一个元素，也就是指示yi是+1还是-1的数据
		vector<double> chooseElement(datas[i].begin(), datas[i].end() - 1);
		double xMulW0 = matrixMultiplicationMatrix(chooseElement, w0);//定义xi和w0的乘积产生的结果
		if (yi * (xMulW0 + b) <= 0) {
			cout <<"选择的分类样本点分别为：" << "x" << i + 1 << endl;
			return pair<vector<double>, bool>(datas[i], false);
		}
	}
	return pair<vector<double>, bool>({}, true);
}


//第四步：根据公式去算就可以了，这里我们先分析一下这个公式是怎么来的
/*感知机算法的损失函数可以用误分类点的总数来衡定，但是这样的损失函数不易优化，所以我们采取另一个措施
* 那就是用误分类点到超平面S的距离，那么总损失函数就是这个函数求和，其中求和的次数就是误分类点的个数
* 我们知道，这个超平面的方程是w * x + b = 0，那么对于任一点x0，距离就是1 / ||w|| * |w * x0 + b|
* ||w||是w的二范数，也就是各项的平方加起来开根号，因为如果是错误分类的话，那么有以下两种情况
* 第一种：如果正确的分类是Y = +1，但是却分类到了Y = -1,那么-y * (w * x + b) > 0
* 第二种：如果正确的分类是Y = -1，但是却分类到了Y = +1,那么-y * (w * x + b) > 0
* 所以我们完全可以用-y * (w * x + b) * 1 / ||w||这个式子来衡量点x到超平面的距离
* 之所以可以乘上y，这是因为y就只有两个值+1和-1，所以乘上并不影响最终的结果。
* 又因为1 / ||w||是一个常数，他不会影响最终的结果。这里我们直接不管。
* 那么最终的损失函数就是-y * (w * x + b)对xi∈M中的所有元素求和。（M即是所有误分类点的集合，xi代表里面的某个元素）
* 
* 
* 我们再回到感知机算法的核心：是为了求一个合适的w和b参数的超平面，用来对所有的数据点进行一个分类
* 所以最重要的就是求w和b了，为了求w和b而且为了让-y * (w * x + b)最小，这里我们采用随即梯队下降法。
* 先对w求偏导，那么就是对-yi * xi从Xi∈M中的所有元素求和。
* 先对b求偏导，那么就是对-yi从Xi∈M中的所有元素求和。
* 每次随机选择一个误分类点，然后对w和b进行更新
* w <- w + ηyi*xi,b <- b + ηyi,其中η我们一般取1
*/
void calculate(vector<vector<double>> datas)
{
	//设置w0和b0的初始值
	vector<double> w0(datas[0].size() - 1, 0);//w0是一个n维的向量,n代表x的维度，即输入数据的维度
	int b0 = 0;

	while (true)
	{

		//拿到所有的数据，并且标记其分类正确与否，如果分类正确，返回空数组 + true，否则，返回这个分类错误的数据 + false
		pair<vector<double>, bool> allDatas = isItClassifiedCorrectly(datas, w0, b0);

		if (!allDatas.second)//如果存在分类错误的数据
		{
			int n = allDatas.first.size() - 1;
			double yi = allDatas.first[n];//拿到最后一个元素，也就是指示yi是+1还是-1的数据
			vector<double> chooseElement(allDatas.first.begin(),allDatas.first.end() - 1);
			vector<double> xiMulYi = matrixMultiplicationConstant(chooseElement, yi);//计算xi * yi的值,因为是常数乘矩阵，所以返回一个矩阵。
			w0 = matrixAdd(w0, xiMulYi);//重新把这个值给到w0
			b0 = b0 + yi;//重新把这个值给到b0
		}

		else
		{
			cout << "经过计算，w的值依次为：" << endl;
			for (int i = 0; i < w0.size(); ++i)
			{
				cout << w0[i] << " ";
			}
			cout << endl;
			cout << "对应的b的值为：" << endl;
			cout << b0 << endl;
			return;
		}
	}
}


int main()
{
	vector<vector<double>> alldatas = getDatas();//获得所有数据
	calculate(alldatas);//调用总方法
}
