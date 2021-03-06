#include <stdio.h>
#include "FileIO.h"
#include "CNNLayer.h"
#include "MathLayer.h"
//#include "Network.h"
#include "AddingNoise.h"
#include "FileIOCSV.h"


#define BATSIZE 32

/*这一部分定义神经网络结构*/
/*=========================================
NetWorkNode:网络节点，B指的是bias

OutputNode: 输出层之后softmax出结果

LossNode：输出各种loss value

NetWorkWeight：各层之间节点权重
=========================================*/

struct NetWorkNode
{

	double Seond1[200];
	double Thired[50];
	double Fourth[10];
	double Seond1B[200];
	double ThiredB[50];
	double FourthB[10];
} NNode;

struct OutputNode
{

	double Exp[10];
	double SumofExp;
	double Prob[10];
	double Loss[10];

} ONode;

struct BackNode
{

	double Db4[10];
	double Db3[50];
	double Db2[200];
	double Seond1[200];
	double Thired[50];
	double Fourth[10];
	double DSeond1[200 * 784];
	double DThired[50 * 200];
	double DFourth[10 * 50];

} BNode;

struct NetWorkbatch
{
	double Seond1Bat[BATSIZE][200 * 784];
	double ThiredBat[BATSIZE][50 * 200];
	double FourthBat[BATSIZE][10 * 50];
	double Seond1BBat[BATSIZE][200];
	double ThiredBBat[BATSIZE][50];
	double FourthBBat[BATSIZE][10];
} NBat;

struct NetWorkWeight
{
	double Seond1[200 * 784];
	double Thired[50 * 200];
	double Fourth[10 * 50];
} NWeight;


/*这一部分定义神经网络结构*/
/*=========================================
NetWorkNode:网络节点，B指的是bias

OutputNode: 输出层之后softmax出结果

LossNode：输出各种loss value

NetWorkWeight：各层之间节点权重

假设： 新网络架构成为 A B C D
=========================================*/

#define NA 784
#define NB 200
#define NC 50
#define ND 10

struct NetWorkNode_N
{

	double Seond1[NB];
	double Thired[NC];
	double Fourth[ND];
	double Seond1B[NB];
	double ThiredB[NC];
	double FourthB[ND];
} NNode_N;

struct OutputNode_N
{

	double Exp[ND];
	double SumofExp;
	double Prob[ND];
	double Loss[ND];

} ONode_N;

struct BackNode_N
{

	double Db4[ND];
	double Db3[NC];
	double Db2[NB];
	double Seond1[NB];
	double Thired[NC];
	double Fourth[ND];
	double DSeond1[NB * NA];
	double DThired[NC* NB];
	double DFourth[ND * NC];

} BNode_N;



struct NetWorkWeight_N
{
	double Seond1[NB * NA];
	double Thired[NC * NB];
	double Fourth[ND * NC];
} NWeight_N;




/*============================Start====================================
Protection:
1.random
2.adding noise
==============================Start===============================*/
/*
struct NetWorkWeightR
{
	double Seond1[200 * 784];
	double Thired[50 * 200];
	double Fourth[10 * 50];
} NWeightR;
struct NetWorkNodeR
{
	double Seond1[200];
	double Thired[50];
	double Fourth[10];
	double Seond1B[200];
	double ThiredB[50];
	double FourthB[10];
} NNodeR;*/
//=============================end====================================


//=====================================================================
/*这一部分定义神经网络结构*/
/*
=============+++++++++++++++
2.307179448830937 [0.10098569 0.11170167 0.09640025 0.10333706 0.09606873 0.09076936
 0.09954162 0.10587555 0.09583138 0.0994887 ] 6
=============+++++++++++++++
*/
int findMax(double prob[10], int dem)
{
	int i = 0;
	int k = 0;
	double TMP = 0;
	for (i = 0; i < dem; i++)
	{
		if (TMP < prob[i])
		{
			TMP = prob[i];
			k = i;
		}
	}
	return k;
}

int DigitNumber(int index)
{
	int Len = 1;
	double tmp = double(index);
	//while (double(double(tmp) / 10.0 )> 1)
	while (tmp / 10.0 >= 1)
	{
		tmp = tmp / 10;
		Len++;
	}
	return Len;
}
void CopyFilename(char *filenameB, char *filenameO)
{
	int i = 25;
	int j = 0;
	for (j = 0; j < i; j++)
	{
		filenameB[j] = filenameO[j];
	}
}
void nameMaker(char *filenameB, int Index)
{
	int i = 0;
	int len = 25;
	int startPoint = 18;
	int insertPoint = 13;
	int digitLen = DigitNumber(Index);
	int tmp;
	int Remain = Index;
	//平移 digitLen-1 个字符 减一是因为原来就有一个数字index
	//printf("=====%s====\n", filenameB);
	for (i = 0; i < startPoint - insertPoint; i++)
	{
		filenameB[startPoint - i + digitLen - 1] = filenameB[startPoint - i];
		//printf("=====%s====\n", filenameB);
	}
	//修改添加内容
	for (i = digitLen; i > 0; i--)
	{
		tmp = Remain % 10;
		filenameB[insertPoint + i - 1] = '0' + tmp;
		Remain = int(Remain / 10);
		//filenameB[startPoint - i] = filenameB[startPoint - i - 1];
		//printf("+++%s++++\n", filenameB);
	}
	//filenameB[insertPoint] = 'C';
}


void nameMakerT(char *filenameB, int Index)
{
	int i = 0;
	int len = 25;
	int startPoint = 17;
	int insertPoint = 12;
	int digitLen = DigitNumber(Index);
	int tmp;
	int Remain = Index;
	//平移 digitLen-1 个字符 减一是因为原来就有一个数字index
	//printf("=====%s====\n", filenameB);
	for (i = 0; i < startPoint - insertPoint; i++)
	{
		filenameB[startPoint - i + digitLen - 1] = filenameB[startPoint - i];
		//printf("=====%s====\n", filenameB);
	}
	//修改添加内容
	for (i = digitLen; i > 0; i--)
	{
		tmp = Remain % 10;
		filenameB[insertPoint + i - 1] = '0' + tmp;
		Remain = int(Remain / 10);
		//filenameB[startPoint - i] = filenameB[startPoint - i - 1];
		//printf("+++%s++++\n", filenameB);
	}
	//filenameB[insertPoint] = 'C';
}

void initNode()
{
	int i;
	for (i = 0; i < 200; i++)
	{
		//NNode.Seond1[i] = 0.1;
		NNode.Seond1[i] = 0.0;
	}
	for (i = 0; i < 50; i++)
	{

		NNode.Thired[i] = 0.0;
	}
	for (i = 0; i < 10; i++)
	{
		NNode.Fourth[i] = 0.0;
	}

}

void initNodeC(int a, int b, int c)
{
	int i;
	for (i = 0; i < a; i++)
	{
		//NNode.Seond1[i] = 0.1;
		NNode.Seond1[i] = 0.0;
	}
	for (i = 0; i < b; i++)
	{

		NNode.Thired[i] = 0.0;
	}
	for (i = 0; i < c; i++)
	{
		NNode.Fourth[i] = 0.0;
	}

}



void initBNode()
{
	int i;
	for (i = 0; i < 200; i++)
	{
		//NNode.Seond1[i] = 0.1;
		BNode.Seond1[i] = 0.0;
	}
	for (i = 0; i < 50; i++)
	{

		BNode.Thired[i] = 0.0;
	}
	for (i = 0; i < 10; i++)
	{
		BNode.Fourth[i] = 0.0;
	}

}

void init()
{
	int i;
	for (i = 0; i < 200; i++)
	{
		//NNode.Seond1[i] = 0.1;
		NNode.Seond1B[i] = 0.1;
	}
	for (i = 0; i < 50; i++)
	{

		NNode.ThiredB[i] = 0.1;
	}
	for (i = 0; i < 10; i++)
	{
		NNode.FourthB[i] = 0.1;
	}

	for (i = 0; i < 200 * 784; i++)
	{
		//NNode.Seond1[i] = 0.1;
		NWeight.Seond1[i] = 0.00001;
	}
	for (i = 0; i < 200 * 50; i++)
	{
		//NNode.Seond1[i] = 0.1;
		NWeight.Thired[i] = 0.00001;
	}
	for (i = 0; i < 10 * 50; i++)
	{
		//NNode.Seond1[i] = 0.1;
		NWeight.Fourth[i] = 0.00001;
	}

}

void initB()
{
	int i;
	int j;
	for (j = 0; j < BATSIZE; j++)
	{
		for (i = 0; i < 200; i++)
		{
			//NNode.Seond1[i] = 0.1;
			NBat.Seond1BBat[j][i] = 0.0;
		}
		for (i = 0; i < 50; i++)
		{

			NBat.ThiredBBat[j][i] = 0.0;
		}
		for (i = 0; i < 10; i++)
		{
			NBat.FourthBBat[j][i] = 0.0;
		}

		for (i = 0; i < 200 * 784; i++)
		{
			//NNode.Seond1[i] = 0.1;
			NBat.Seond1Bat[j][i] = 0.0;
		}
		for (i = 0; i < 200 * 50; i++)
		{
			//NNode.Seond1[i] = 0.1;
			NBat.ThiredBat[j][i] = 0.0;
		}
		for (i = 0; i < 10 * 50; i++)
		{
			//NNode.Seond1[i] = 0.1;
			NBat.FourthBat[j][i] = 0.0;
		}
	}


}


void BatchSum()
{
	int i;
	int j;


	double tmp;
	double norm = 1;
	for (i = 0; i < 200; i++)
	{
		tmp = 0;
		for (j = 0; j < BATSIZE; j++)
		{
			tmp = NBat.Seond1BBat[j][i] + tmp;
		}
		//NNode.Seond1[i] = 0.1;
		NNode.Seond1B[i] = tmp / norm;
	}
	for (i = 0; i < 50; i++)
	{
		tmp = 0;
		for (j = 0; j < BATSIZE; j++)
		{
			tmp = NBat.ThiredBBat[j][i] + tmp;
		}
		NNode.ThiredB[i] = tmp / norm;
	}
	for (i = 0; i < 10; i++)
	{
		tmp = 0;
		for (j = 0; j < BATSIZE; j++)
		{
			tmp = NBat.FourthBBat[j][i] + tmp;
		}
		NNode.FourthB[i] = tmp / norm;
	}

	for (i = 0; i < 200 * 784; i++)
	{
		tmp = 0;
		for (j = 0; j < BATSIZE; j++)
		{
			tmp = NBat.Seond1Bat[j][i] + tmp;
		}
		//NNode.Seond1[i] = 0.1;
		NWeight.Seond1[i] = tmp / norm;
	}
	for (i = 0; i < 200 * 50; i++)
	{
		tmp = 0;
		for (j = 0; j < BATSIZE; j++)
		{
			tmp = NBat.ThiredBat[j][i] + tmp;
		}
		//NNode.Seond1[i] = 0.1;
		NWeight.Thired[i] = tmp / norm;
	}
	for (i = 0; i < 10 * 50; i++)
	{
		tmp = 0;
		for (j = 0; j < BATSIZE; j++)
		{
			tmp = NBat.FourthBat[j][i] + tmp;
		}
		//NNode.Seond1[i] = 0.1;
		NWeight.Fourth[i] = tmp / norm;
	}



}




/*第一版，可以进行神经网络识别手写数字的代码*/
/*V1*/
void mainV1()
{
	printf("Test using existing weight!\n");

	char WeightWA[25] = "001_WA_F.csv";
	char WeightWB[25] = "001_WB_F.csv";
	char WeightWC[25] = "001_WC_F.csv";
	char WeightBAC[25] = "001_BA_F.csv";
	char WeightBBC[25] = "001_BB_F.csv";
	char WeightBCC[25] = "001_BC_F.csv";
	int count = 0;
	count = ReadWeightAndBiasFromFiles(WeightWA, NWeight.Seond1, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightWB, NWeight.Thired, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightWC, NWeight.Fourth, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBAC, NNode.Seond1B, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBBC, NNode.ThiredB, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBCC, NNode.FourthB, 784, 200);


	//=========================准备训练图片和测试图片的label==================
	char filenameTrainLabel[25] = "TrainingLabel.txt";
	char filenameTeestLabeel[25] = "TestingLabel.txt";
	int TrainLabel[60000];
	int TestLabel[10000];
	TReadImageMNistLabel(filenameTrainLabel, TrainLabel);
	TReadImageMNistLabel(filenameTeestLabeel, TestLabel);

	//=================准备训练图片和测试图片=================
	double imageB[784] = { 0 };
	//ReadImage(filename);
	//image=ReadImageMNist(filename);
	char filenameO[25] = "TrainingImage0.txt";
	char filenameB[25] = "TrainingImage0.txt";
	char filenameOT[25] = "TestingImage0.txt";
	char filenameBT[25] = "TestingImage0.txt";
	int countImages = 60000;
	int testImages = 10000;
	int countImageIndex = 1;

	int Pre_Res;
	int T;
	int kID;
	int AccCount = 0;
	int i, j;
	for (countImageIndex = 0; countImageIndex < testImages; countImageIndex++)
	{
		CopyFilename(filenameBT, filenameOT);
		nameMakerT(filenameBT, countImageIndex);
		//printf("obtain the file of: %s     \n", filenameB);
		TReadImageMNistC(filenameBT, imageB);
		/*
		for (kID = 0; kID < 784; kID++)
		{
			//printf("%d     %f\n", kID, imageB[kID]);
			printf("%d     %f\n", kID, NWeight.Seond1[kID]);

		}
		*/
		//==========forward propagation================
		initNode();
		FClayerForward(imageB, NNode.Seond1, NWeight.Seond1, 784, 200, NNode.Seond1B);
		FClayerForwardRelu(NNode.Seond1, 200);
		FClayerForward(NNode.Seond1, NNode.Thired, NWeight.Thired, 200, 50, NNode.ThiredB);
		FClayerForwardRelu(NNode.Thired, 50);
		FClayerForward(NNode.Thired, NNode.Fourth, NWeight.Fourth, 50, 10, NNode.FourthB);
		//==========calculate the loss deviation================
		//ONode.SumofExp = ExpSumToProb(NNode.Fourth, ONode.Exp , 10, ONode.Prob);
		Pre_Res = findMax(NNode.Fourth, 10);
		T = TestLabel[countImageIndex];//获取groundtruth

		if (Pre_Res == T)
		{
			AccCount++;
			//printf("=====%d   ---------%d\n", Pre_Res, T);
		}


	}
	double Acc = double(double(AccCount) / double(testImages));
	printf("The recognition accuracy on CPU is %f\n", Acc);

	/*============================Start====================================
	Protection:
	1.random
	2.adding noise
	==============================Start===============================*/
	printf("==========reordering and noise===\n");

	while (1)
	{
		;
	}
}





/*基于第一版，可以进行神经网络识别手写数字的代码， 修改满足DAC项目的代码*/
// 先来选点

#include <cmath>
#include <cstdlib>

#define NK 1
#define NMA 78
#define NMB 40
#define NMC 15
#define NPA 4
#define NPB 4
#define NPC 4
#define TestK  false //false true

double WeightTmp[784][NK];
int NodeIndA[NMA*NPA];
int NodeIndB[NMB*NPB];
int NodeIndC[NMC*NPC];

void selectTopK(int PDemA,int CDemB, double * weightO)
{
	

	int i = 0;
	int j = 0;
	if (TestK == true)
	{
		//=============test rand()======================A
		for (i = 0; i < CDemB; i++)
		{
			NWeight.Seond1[PDemA*CDemB + i] = rand();
			printf(" %f,", NWeight.Seond1[PDemA*CDemB + i]);
		}
		printf("\n");
		//=============test rand()======================A
	}
	
	
	for (i = 0; i < NK; i++)
	{
		WeightTmp[PDemA][i] = 0;
	}
	double Tmp=0;
	double SmallK = 0;
	int SmallKInd = 0;
	for (i = 0; i < CDemB; i++)
	{
		//Tmp= fabs(NWeight.Seond1[PDemA*CDemB+i]);
		Tmp = fabs(weightO[PDemA*CDemB + i]);
		if (Tmp >SmallK)
		{
			WeightTmp[PDemA][SmallKInd] = Tmp;
		}
		Tmp = 65536;
		for (j = 0; j < NK; j++)
		{
			if (Tmp > WeightTmp[PDemA][j])
			{
				Tmp = WeightTmp[PDemA][j];
				SmallKInd = j;
			}
		}
		SmallK = Tmp;
	}
	if (TestK == true)
	{
		//=============test rand()======================A
		for (i = 0; i < NK; i++)
		{
			printf(" %f,", WeightTmp[PDemA][i]);
		}
		printf("\n");
		//=============test rand()======================A
	}
}

void SumTopK(int PDemA, int CDemB)
{
	int j = 0;
	for (j = 1; j < NK; j++)
	{
		WeightTmp[j][0] = WeightTmp[j][0] + WeightTmp[j][j];
	}
}
double MaxNode[800];
void SaveNode(int * NodeInd,int PDemA)//,  int Flag
{
	
	//int MaxNodeInd[NM];
	int i;
	int j;
	int LenNM = 0;
	int Flag = 1;
	switch (Flag)
	{
	case 1: LenNM = NMA; break;
	case 2: LenNM = NMB; break;
	case 3: LenNM = NMC; break;
	default:
		break;
	}
	for (i = 0; i < LenNM; i++)
	{
		MaxNode[i] = 0;
	}
	double Tmp = 0;
	double SmallK = 0;
	int SmallKInd = 0;
	for (i = 0; i < PDemA; i++)
	{
		Tmp = WeightTmp[i][0];
		if (Tmp > SmallK)
		{
			MaxNode[SmallKInd] = Tmp;
			//MaxNodeInd[SmallKInd] = i;
			NodeInd[SmallKInd] = i;
		}
		Tmp = 65536;
		for (j = 0; j < NMA; j++)
		{
			if (Tmp > MaxNode[j])
			{
				Tmp = MaxNode[j];
				SmallKInd = j;
			}
		}
		SmallK = Tmp;
	}
}

void select_node()
{


	
	double SelectNod[784];

	/*===========================
	===========Step 1:  选出关键点 
	============================*/
	int i = 0;
	int j = 0;
	//=============================layer 1====================================
	//============== 1 统计信息
	//selectTopK(1, 20);
	for (i = 0; i < 784; i++)
	{
		//NWeight.Seond1[ ]
		//printf("Woorking \n");
		selectTopK(i, 200, NWeight.Seond1);	//选topK的权重 并存起来
		SumTopK(i, 200);
	}
	
	//=========== 2 从统计信息中选关键点
	SaveNode(NodeIndA, 784);

	//=============================layer 2====================================
	//============== 1 统计信息
	//selectTopK(1, 20);
	for (i = 0; i < 200; i++)
	{
		//NWeight.Seond1[ ]
		//printf("Woorking \n");
		selectTopK(i, 50, NWeight.Thired);	//选topK的权重 并存起来
		SumTopK(i, 50);
	}

	//=========== 2 从统计信息中选关键点
	SaveNode(NodeIndC, 200);

	//=============================layer 3====================================
	//============== 1 统计信息
	//selectTopK(1, 20);
	for (i = 0; i < 50; i++)
	{
		//NWeight.Seond1[ ]
		//printf("Woorking \n");
		selectTopK(i, 10, NWeight.Fourth);	//选topK的权重 并存起来
		SumTopK(i, 10);
	}

	//=========== 2 从统计信息中选关键点
	SaveNode(NodeIndB, 50);
	
}

bool ChcekiniF(int Len, int * ListName, int Target)
{
	int i = 0;
	for (i = 0; i < Len; i++)
	{
		if (Target == ListName[i])
		{
			return true;
		}
	}
	return false;
}

#define NM 10
#define NP 10
void ModifyWeight()
{
	int i = 0;
	int j = 0;

	/*===========================
	===========Step 2:  改权重
	============================*/
	int PrDem = 784;
	int CurDem = 200;
	//拷贝原值
	for (i = 0; i < PrDem; i++)
	{
		for (j = 0; j < CurDem; j++)
		{
			NWeight_N.Seond1[i*CurDem+j] = NWeight.Seond1[i*CurDem + j];
		}
	}
	for (i = PrDem; i < NM + PrDem; i++)
	{
		for (j = 0; j < CurDem; j++)
		{
			NWeight_N.Seond1[NodeIndA[i]*CurDem + j] = NWeight.Seond1[NodeIndA[i]*CurDem + j]/NP;
		}
	}
	//拷贝扩展值，只针对当前权重
	for (i = PrDem; i < PrDem; i++)
	{
		for (j = 0; j < CurDem; j++)
		{
			NWeight_N.Seond1[i*CurDem + j] = NWeight.Seond1[i*CurDem + j];
		}
	}
	//拷贝原值
	for (i = 0; i < PrDem; i++)
	{
		for (j = 0; j < CurDem; j++)
		{
			NWeight_N.Seond1[i*CurDem + j] = NWeight.Seond1[i*CurDem + j];
		}
	}
}

/*DAC_V1*/
void mainV1_Dac()
{
	printf("Test using existing weight!\n");

	char WeightWA[25] = "001_WA_F.csv";
	char WeightWB[25] = "001_WB_F.csv";
	char WeightWC[25] = "001_WC_F.csv";
	char WeightBAC[25] = "001_BA_F.csv";
	char WeightBBC[25] = "001_BB_F.csv";
	char WeightBCC[25] = "001_BC_F.csv";
	int count = 0;
	count = ReadWeightAndBiasFromFiles(WeightWA, NWeight.Seond1, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightWB, NWeight.Thired, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightWC, NWeight.Fourth, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBAC, NNode.Seond1B, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBBC, NNode.ThiredB, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBCC, NNode.FourthB, 784, 200);


	//=========================准备训练图片和测试图片的label==================
	char filenameTrainLabel[25] = "TrainingLabel.txt";
	char filenameTeestLabeel[25] = "TestingLabel.txt";
	int TrainLabel[60000];
	int TestLabel[10000];
	TReadImageMNistLabel(filenameTrainLabel, TrainLabel);
	TReadImageMNistLabel(filenameTeestLabeel, TestLabel);

	//=================准备训练图片和测试图片=================
	double imageB[784] = { 0 };
	//ReadImage(filename);
	//image=ReadImageMNist(filename);
	char filenameO[25] = "TrainingImage0.txt";
	char filenameB[25] = "TrainingImage0.txt";
	char filenameOT[25] = "TestingImage0.txt";
	char filenameBT[25] = "TestingImage0.txt";
	int countImages = 60000;
	int testImages = 10000;
	int countImageIndex = 1;

	int Pre_Res;
	int T;
	int kID;
	int AccCount = 0;
	int i, j;
	for (countImageIndex = 0; countImageIndex < testImages; countImageIndex++)
	{
		CopyFilename(filenameBT, filenameOT);
		nameMakerT(filenameBT, countImageIndex);
		//printf("obtain the file of: %s     \n", filenameB);
		TReadImageMNistC(filenameBT, imageB);
		/*
		for (kID = 0; kID < 784; kID++)
		{
			//printf("%d     %f\n", kID, imageB[kID]);
			printf("%d     %f\n", kID, NWeight.Seond1[kID]);

		}
		*/
		//==========forward propagation================
		initNode();
		FClayerForward(imageB, NNode.Seond1, NWeight.Seond1, 784, 200, NNode.Seond1B);
		FClayerForwardRelu(NNode.Seond1, 200);
		FClayerForward(NNode.Seond1, NNode.Thired, NWeight.Thired, 200, 50, NNode.ThiredB);
		FClayerForwardRelu(NNode.Thired, 50);
		FClayerForward(NNode.Thired, NNode.Fourth, NWeight.Fourth, 50, 10, NNode.FourthB);
		//==========calculate the loss deviation================
		//ONode.SumofExp = ExpSumToProb(NNode.Fourth, ONode.Exp , 10, ONode.Prob);
		Pre_Res = findMax(NNode.Fourth, 10);
		T = TestLabel[countImageIndex];//获取groundtruth

		if (Pre_Res == T)
		{
			AccCount++;
			//printf("=====%d   ---------%d\n", Pre_Res, T);
		}


	}
	double Acc = double(double(AccCount) / double(testImages));
	printf("The recognition accuracy on CPU is %f\n", Acc);

	/*============================Start====================================
	Protection:
	1.random
	2.adding noise
	==============================Start===============================*/
	printf("==========reordering and noise===\n");

	while (1)
	{
		;
	}
}





/*测试 fault injection*/
/* V2 单次检测成功*/

void mainV2()
{
	printf("Test using existing weight!\n");

	char WeightWA[25] = "001_WA_F.csv";
	char WeightWB[25] = "001_WB_F.csv";
	char WeightWC[25] = "001_WC_F.csv";
	char WeightBAC[25] = "001_BA_F.csv";
	char WeightBBC[25] = "001_BB_F.csv";
	char WeightBCC[25] = "001_BC_F.csv";
	int count = 0;
	count = ReadWeightAndBiasFromFiles(WeightWA, NWeight.Seond1, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightWB, NWeight.Thired, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightWC, NWeight.Fourth, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBAC, NNode.Seond1B, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBBC, NNode.ThiredB, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBCC, NNode.FourthB, 784, 200);


	/*============================Start====================================
	Protection:
	1.random
	2.adding noise
	==============================Start===============================*/
	printf("==========adding noise===\n");

	int NumbOfNoise = 100;
	int AddingType = 0;   //0 randomly, 1->important first
	int Welen = 200 * 784;
	AddingNoise(NWeight.Seond1, NumbOfNoise, AddingType, Welen);
	//NWeight.Fourth;
	Welen = 10*50;
	AddingNoise(NWeight.Fourth, NumbOfNoise, AddingType, Welen);

	printf("==========adding noise  done===\n");
	//=========================准备训练图片和测试图片的label==================
	char filenameTrainLabel[25] = "TrainingLabel.txt";
	char filenameTeestLabeel[25] = "TestingLabel.txt";
	int TrainLabel[60000];
	int TestLabel[10000];
	TReadImageMNistLabel(filenameTrainLabel, TrainLabel);
	TReadImageMNistLabel(filenameTeestLabeel, TestLabel);

	//=================准备训练图片和测试图片=================
	double imageB[784] = { 0 };
	//ReadImage(filename);
	//image=ReadImageMNist(filename);
	char filenameO[25] = "TrainingImage0.txt";
	char filenameB[25] = "TrainingImage0.txt";
	char filenameOT[25] = "TestingImage0.txt";
	char filenameBT[25] = "TestingImage0.txt";
	int countImages = 60000;
	int testImages = 10000;
	int countImageIndex = 1;

	int Pre_Res;
	int T;
	int kID;
	int AccCount = 0;
	int i, j;
	for (countImageIndex = 0; countImageIndex < testImages; countImageIndex++)
	{
		CopyFilename(filenameBT, filenameOT);
		nameMakerT(filenameBT, countImageIndex);
		//printf("obtain the file of: %s     \n", filenameB);
		TReadImageMNistC(filenameBT, imageB);
		/*
		for (kID = 0; kID < 784; kID++)
		{
			//printf("%d     %f\n", kID, imageB[kID]);
			printf("%d     %f\n", kID, NWeight.Seond1[kID]);

		}
		*/
		//==========forward propagation================
		initNode();
		FClayerForward(imageB, NNode.Seond1, NWeight.Seond1, 784, 200, NNode.Seond1B);
		FClayerForwardRelu(NNode.Seond1, 200);
		FClayerForward(NNode.Seond1, NNode.Thired, NWeight.Thired, 200, 50, NNode.ThiredB);
		FClayerForwardRelu(NNode.Thired, 50);
		FClayerForward(NNode.Thired, NNode.Fourth, NWeight.Fourth, 50, 10, NNode.FourthB);
		//==========calculate the loss deviation================
		//ONode.SumofExp = ExpSumToProb(NNode.Fourth, ONode.Exp , 10, ONode.Prob);
		Pre_Res = findMax(NNode.Fourth, 10);
		T = TestLabel[countImageIndex];//获取groundtruth

		if (Pre_Res == T)
		{
			AccCount++;
			//printf("=====%d   ---------%d\n", Pre_Res, T);
		}


	}
	double Acc = double(double(AccCount) / double(testImages));
	printf("The recognition accuracy on CPU is %f\n", Acc);

	
	while (1)
	{
		;
	}
}






void mainK()
{
	//读取模型权重文件
	//  这一部分在CPU端，读取权重文件
	printf("Test using existing weight!\n");
	
	char WeightWA[25] = "001_WA_F.csv";
	char WeightWB[25] = "001_WB_F.csv";
	char WeightWC[25] = "001_WC_F.csv";
	char WeightBAC[25] = "001_BA_F.csv";
	char WeightBBC[25] = "001_BB_F.csv";
	char WeightBCC[25] = "001_BC_F.csv";
	int count = 0;
	count = ReadWeightAndBiasFromFiles(WeightWA, NWeight.Seond1, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightWB, NWeight.Thired, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightWC, NWeight.Fourth, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBAC, NNode.Seond1B, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBBC, NNode.ThiredB, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBCC, NNode.FourthB, 784, 200);

	//=========================准备训练图片和测试图片的label==================
	char filenameTrainLabel[25] = "TrainingLabel.txt";
	char filenameTeestLabeel[25] = "TestingLabel.txt";
	int TrainLabel[60000];
	int TestLabel[10000];
	TReadImageMNistLabel(filenameTrainLabel, TrainLabel);
	TReadImageMNistLabel(filenameTeestLabeel, TestLabel);

	//=================准备训练图片和测试图片=================
	double imageB[784] = { 0 };
	//ReadImage(filename);
	//image=ReadImageMNist(filename);
	char filenameO[25] = "TrainingImage0.txt";
	char filenameB[25] = "TrainingImage0.txt";
	char filenameOT[25] = "TestingImage0.txt";
	char filenameBT[25] = "TestingImage0.txt";
	int countImages = 60000;
	int testImages = 10000;
	int countImageIndex = 1;

	int Pre_Res;
	int T;
	int kID;
	int AccCount = 0;
	int i, j;
	for (countImageIndex = 0; countImageIndex < testImages; countImageIndex++)
	{
		CopyFilename(filenameBT, filenameOT);
		nameMakerT(filenameBT, countImageIndex);
		//printf("obtain the file of: %s     \n", filenameB);
		TReadImageMNistC(filenameBT, imageB);
		/*
		for (kID = 0; kID < 784; kID++)
		{
			//printf("%d     %f\n", kID, imageB[kID]);
			printf("%d     %f\n", kID, NWeight.Seond1[kID]);

		}
		*/
		//==========forward propagation================
		initNode();
		FClayerForward(imageB, NNode.Seond1, NWeight.Seond1, 784, 200, NNode.Seond1B);
		FClayerForwardRelu(NNode.Seond1, 200);
		FClayerForward(NNode.Seond1, NNode.Thired, NWeight.Thired, 200, 50, NNode.ThiredB);
		FClayerForwardRelu(NNode.Thired, 50);
		FClayerForward(NNode.Thired, NNode.Fourth, NWeight.Fourth, 50, 10, NNode.FourthB);
		//==========calculate the loss deviation================
		//ONode.SumofExp = ExpSumToProb(NNode.Fourth, ONode.Exp , 10, ONode.Prob);
		Pre_Res = findMax(NNode.Fourth, 10);
		T = TestLabel[countImageIndex];//获取groundtruth

		if (Pre_Res == T)
		{
			AccCount++;
			//printf("=====%d   ---------%d\n", Pre_Res, T);
		}


	}
	double Acc = double(double(AccCount) / double(testImages));
	printf("The recognition accuracy on CPU is %f\n", Acc);



	/*============================Start====================================
	Protection:
	1.random
	2.adding noise
	==============================Start===============================*/
	printf("==========reordering and noise===\n");
	while (1)
	{
		;
	}
	//return 0;

	//=============================end====================================

	//================evaluate the FPGA part=======================================

	double Weight[784 * 200 + 200 * 50];
	double Bias[200 + 50];

	for (i = 0; i < 784 * 200;i++)
	{
		Weight[i] = NWeight.Seond1[i];
	}
	for (i = 0; i < 50 * 200; i++)
	{
		Weight[i+ 784 * 200] = NWeight.Thired[i];
	}

	for (i = 0; i <  200; i++)
	{
		Bias[i] = NNode.Seond1B[i];
	}
	for (i = 0; i < 50; i++)
	{
		Bias[i + 200] = NNode.ThiredB[i];
	}



	/*
	夏可：
	上部分包括了CPU部分对LENET的测试，然后初始化了权重buff（Weight）和偏置buff （Bias）
	对应FPGA ip内部 InputDataW 和 InputBais
	下面一个你要先初始化一下， flag为1 初始化权重和偏置。TODO NO1.
	然后进入一个for循环，测试所有测试图片。
	在for循环中，先获取视频，然后flag=2输入图片， TODO NO2 InputDataM 对应imageB
	最后运行 FPGA的IP，，将输出的结果放入最后权重里面计算最后输出。 TODO NO3

	*/

	//============= flag为1 初始化权重和偏置=================NO1===Start=======

	//============= flag为1 初始化权重和偏置=================NO1===END=======

	AccCount = 0;
	for (countImageIndex = 0; countImageIndex < testImages; countImageIndex++)
	{
		CopyFilename(filenameBT, filenameOT);
		nameMakerT(filenameBT, countImageIndex);
		//printf("obtain the file of: %s     \n", filenameB);
		TReadImageMNistC(filenameBT, imageB);

		//============= flag=2输入图片=================NO2===Start=======
		// imageB 就相当于 FPGA IP里面 InputDataM

		//============= flag=2输入图片=================NO2===END=======

		/*
		for (kID = 0; kID < 784; kID++)
		{
			//printf("%d     %f\n", kID, imageB[kID]);
			printf("%d     %f\n", kID, NWeight.Seond1[kID]);

		}
		*/
		//==========forward propagation================
		initNode();
		//注释掉前三层放入FPGA端
		//FClayerForward(imageB, NNode.Seond1, NWeight.Seond1, 784, 200, NNode.Seond1B);
		//FClayerForwardRelu(NNode.Seond1, 200);
		//FClayerForward(NNode.Seond1, NNode.Thired, NWeight.Thired, 200, 50, NNode.ThiredB);
		//FClayerForwardRelu(NNode.Thired, 50);

		//============= 最后运行 FPGA的IP=================NO3===Start=======
		
		
		
		// OutputData 的值保存到 NNode.Thired 长度为50
		//这里加一个50的for循环保存数据

		//============= 最后运行 FPGA的IP=================NO3===END=======

		//完成后最后一层在本地完成计算。
		FClayerForward(NNode.Thired, NNode.Fourth, NWeight.Fourth, 50, 10, NNode.FourthB);
		//==========calculate the loss deviation================
		//ONode.SumofExp = ExpSumToProb(NNode.Fourth, ONode.Exp , 10, ONode.Prob);
		Pre_Res = findMax(NNode.Fourth, 10);
		T = TestLabel[countImageIndex];//获取groundtruth

		if (Pre_Res == T)
		{
			AccCount++;
			//printf("=====%d   ---------%d\n", Pre_Res, T);
		}


	}
	Acc = double(double(AccCount) / double(testImages));
	printf("The recognition accuracy on CPU is %f\n", Acc);




	printf("=====%d", count);
	while (1)
	{
		;
	}
}





/* V3, fault injection with multiple test and collect the result*/

double FunctionAddNoise(int NO, int Type, int layA, int layB, int layC, int numberA, int numberB, int numberC)
{
	/*
		NO: 总测试次数
		Type: 0:随机添加， 1 最重要的添加
		layA,layB,layC 对三层网络节点添加的标志
		numberA,B,C 添加数目。
	*/
	printf("Test using existing weight!\n");

	char WeightWA[25] = "001_WA_F.csv";
	char WeightWB[25] = "001_WB_F.csv";
	char WeightWC[25] = "001_WC_F.csv";
	char WeightBAC[25] = "001_BA_F.csv";
	char WeightBBC[25] = "001_BB_F.csv";
	char WeightBCC[25] = "001_BC_F.csv";
	int count = 0;
	count = ReadWeightAndBiasFromFiles(WeightWA, NWeight.Seond1, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightWB, NWeight.Thired, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightWC, NWeight.Fourth, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBAC, NNode.Seond1B, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBBC, NNode.ThiredB, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBCC, NNode.FourthB, 784, 200);


	/*============================Start====================================
	Protection:
	1.random
	2.adding noise
	==============================Start===============================*/
	printf("\n");
	printf("======%d====adding noise==%d=======\n",NO, numberA);
	printf("\n");
	int NumbOfNoise = 100;
	int AddingType = Type;   //0 randomly, 1->important first
	int Welen = 200 * 784;
	if (layA)
	{
		AddingNoise(NWeight.Seond1, numberA, AddingType, Welen);
	}
	if (layB)
	{
		Welen = 200 * 50;
		AddingNoise(NWeight.Thired, numberB, AddingType, Welen);
	}
	if (layC)
	{
		Welen = 10 * 50;
		AddingNoise(NWeight.Fourth, numberC, AddingType, Welen);
	}
	//NWeight.Fourth;
	
	

	printf("==========adding noise  done===\n");
	//=========================准备训练图片和测试图片的label==================
	char filenameTrainLabel[25] = "TrainingLabel.txt";
	char filenameTeestLabeel[25] = "TestingLabel.txt";
	int TrainLabel[60000];
	int TestLabel[10000];
	TReadImageMNistLabel(filenameTrainLabel, TrainLabel);
	TReadImageMNistLabel(filenameTeestLabeel, TestLabel);

	//=================准备训练图片和测试图片=================
	double imageB[784] = { 0 };
	//ReadImage(filename);
	//image=ReadImageMNist(filename);
	char filenameO[25] = "TrainingImage0.txt";
	char filenameB[25] = "TrainingImage0.txt";
	char filenameOT[25] = "TestingImage0.txt";
	char filenameBT[25] = "TestingImage0.txt";
	int countImages = 60000;
	int testImages = 10000;
	int countImageIndex = 1;

	int Pre_Res;
	int T;
	int kID;
	int AccCount = 0;
	int i, j;
	for (countImageIndex = 0; countImageIndex < testImages; countImageIndex++)
	{
		CopyFilename(filenameBT, filenameOT);
		nameMakerT(filenameBT, countImageIndex);
		//printf("obtain the file of: %s     \n", filenameB);
		TReadImageMNistC(filenameBT, imageB);
		/*
		for (kID = 0; kID < 784; kID++)
		{
			//printf("%d     %f\n", kID, imageB[kID]);
			printf("%d     %f\n", kID, NWeight.Seond1[kID]);

		}
		*/
		//==========forward propagation================
		initNode();
		FClayerForward(imageB, NNode.Seond1, NWeight.Seond1, 784, 200, NNode.Seond1B);
		FClayerForwardRelu(NNode.Seond1, 200);
		FClayerForward(NNode.Seond1, NNode.Thired, NWeight.Thired, 200, 50, NNode.ThiredB);
		FClayerForwardRelu(NNode.Thired, 50);
		FClayerForward(NNode.Thired, NNode.Fourth, NWeight.Fourth, 50, 10, NNode.FourthB);
		//==========calculate the loss deviation================
		//ONode.SumofExp = ExpSumToProb(NNode.Fourth, ONode.Exp , 10, ONode.Prob);
		Pre_Res = findMax(NNode.Fourth, 10);
		T = TestLabel[countImageIndex];//获取groundtruth

		if (Pre_Res == T)
		{
			AccCount++;
			//printf("=====%d   ---------%d\n", Pre_Res, T);
		}


	}
	double Acc = double(double(AccCount) / double(testImages));
	printf("The recognition accuracy on CPU is %f\n", Acc);
	return Acc;

}

/* V4, fault injection with multiple test and collect the result*/

double FunctionAddNoiseAndDefence(int NO, int Type, int layA, int layB, int layC, int numberA, int numberB, int numberC, int DefLeng)
{
	/*
		NO: 总测试次数
		Type: 0:随机添加， 1 最重要的添加
		layA,layB,layC 对三层网络节点添加的标志
		numberA,B,C 添加数目。
	*/
	printf("Test using existing weight!\n");

	char WeightWA[25] = "001_WA_F.csv";
	char WeightWB[25] = "001_WB_F.csv";
	char WeightWC[25] = "001_WC_F.csv";
	char WeightBAC[25] = "001_BA_F.csv";
	char WeightBBC[25] = "001_BB_F.csv";
	char WeightBCC[25] = "001_BC_F.csv";
	int count = 0;
	count = ReadWeightAndBiasFromFiles(WeightWA, NWeight.Seond1, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightWB, NWeight.Thired, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightWC, NWeight.Fourth, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBAC, NNode.Seond1B, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBBC, NNode.ThiredB, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBCC, NNode.FourthB, 784, 200);


	/*============================Start====================================
	Protection:
	1.random
	2.adding noise
	==============================Start===============================*/
	printf("\n");
	printf("======%d====adding noise==%d=======\n", NO, numberA);
	printf("\n");
	int NumbOfNoise = 100;
	int AddingType = Type;   //0 randomly, 1->important first
	int Welen = 200 * 784;
	if (layA)
	{
		AddingNoiseDef(NWeight.Seond1, numberA, AddingType, Welen, DefLeng);
	}
	if (layB)
	{
		Welen = 200 * 50;
		AddingNoiseDef(NWeight.Thired, numberB, AddingType, Welen, DefLeng);
	}
	if (layC)
	{
		Welen = 10 * 50;
		AddingNoiseDef(NWeight.Fourth, numberC, AddingType, Welen, DefLeng);
	}
	//NWeight.Fourth;



	printf("==========adding noise  done===\n");
	//=========================准备训练图片和测试图片的label==================
	char filenameTrainLabel[25] = "TrainingLabel.txt";
	char filenameTeestLabeel[25] = "TestingLabel.txt";
	int TrainLabel[60000];
	int TestLabel[10000];
	TReadImageMNistLabel(filenameTrainLabel, TrainLabel);
	TReadImageMNistLabel(filenameTeestLabeel, TestLabel);

	//=================准备训练图片和测试图片=================
	double imageB[784] = { 0 };
	//ReadImage(filename);
	//image=ReadImageMNist(filename);
	char filenameO[25] = "TrainingImage0.txt";
	char filenameB[25] = "TrainingImage0.txt";
	char filenameOT[25] = "TestingImage0.txt";
	char filenameBT[25] = "TestingImage0.txt";
	int countImages = 60000;
	int testImages = 10000;
	int countImageIndex = 1;

	int Pre_Res;
	int T;
	int kID;
	int AccCount = 0;
	int i, j;
	for (countImageIndex = 0; countImageIndex < testImages; countImageIndex++)
	{
		CopyFilename(filenameBT, filenameOT);
		nameMakerT(filenameBT, countImageIndex);
		//printf("obtain the file of: %s     \n", filenameB);
		TReadImageMNistC(filenameBT, imageB);
		/*
		for (kID = 0; kID < 784; kID++)
		{
			//printf("%d     %f\n", kID, imageB[kID]);
			printf("%d     %f\n", kID, NWeight.Seond1[kID]);

		}
		*/
		//==========forward propagation================
		initNode();
		FClayerForward(imageB, NNode.Seond1, NWeight.Seond1, 784, 200, NNode.Seond1B);
		FClayerForwardRelu(NNode.Seond1, 200);
		FClayerForward(NNode.Seond1, NNode.Thired, NWeight.Thired, 200, 50, NNode.ThiredB);
		FClayerForwardRelu(NNode.Thired, 50);
		FClayerForward(NNode.Thired, NNode.Fourth, NWeight.Fourth, 50, 10, NNode.FourthB);
		//==========calculate the loss deviation================
		//ONode.SumofExp = ExpSumToProb(NNode.Fourth, ONode.Exp , 10, ONode.Prob);
		Pre_Res = findMax(NNode.Fourth, 10);
		T = TestLabel[countImageIndex];//获取groundtruth

		if (Pre_Res == T)
		{
			AccCount++;
			//printf("=====%d   ---------%d\n", Pre_Res, T);
		}


	}
	double Acc = double(double(AccCount) / double(testImages));
	printf("The recognition accuracy on CPU is %f\n", Acc);
	return Acc;

}

//可以用来收集一波。

void mainV7()

{
	printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n");
	int TotSizeLayer[3] = { 784 * 200, 200 * 50, 50 * 10 };
	int PortionA[9] = { 1,2,3,4,5,6,7,8,9 };
	int PortionB[9] = { 1,2,3,4,5,6,7,8,9 };
	int PortionDef[9] = { 1,2,3,4,5,6,7,8,9 };
	int TestDefPort = 5;
	double Result[1000];
	double TMPres = 0;
	int NO = 0;
	int layA; 
	int layB; 
	int layC;
	//三个层
/*
	//WriteTOCSV();
	double Test[10] = { 1,2,3,4,5,6,7,8,9,0 };
	AddingNoise(Test, 5, 1, 10);
	for (int i = 0; i < 10; i++)
	{
		printf("==%f==,", Test[i]);
	}
	
	while (1)
	{
		;
	}
	
	*/

	for (int i = 0; i < 3; i++)
	{
		if (i == 0)
		{
			layA = 1;
			layB = 0;
			layC = 0;
		}
		if (i == 1)
		{
			layA = 0;
			layB = 1;
			layC = 0;
		}
		if (i == 2)
		{
			layA = 0;
			layB = 0;
			layC = 1;
		}
		//只攻击的
		/*
		for (int j = 0; j < 9; j++)
		{
			int number = int( TotSizeLayer[i] * PortionA[j] / 10);
			TMPres = FunctionAddNoise(NO, 0, layA, layB, layC, number, number, number);
			Result[NO] = TMPres;
			NO++;
		}
		for (int j = 0; j <9; j++)
		{
			int number = int(TotSizeLayer[i] * PortionB[j] / 100);
			TMPres = FunctionAddNoise(NO, 1, layA, layB, layC, number, number, number);
			Result[NO] = TMPres;
			NO++;
		}
*/
		//只防御
		for (int j = 0; j < 9; j++)
		{
			int number = int(TotSizeLayer[i] * PortionA[j] / 10);
			int numberB = int(TotSizeLayer[i] * PortionA[j] / 10 * TestDefPort/10);
			TMPres = FunctionAddNoiseAndDefence(NO, 0, layA, layB, layC, number, number, number, numberB);
			Result[NO] = TMPres;
			NO++;
		}
		for (int j = 0; j < 9; j++)
		{
			int number = int(TotSizeLayer[i] * PortionB[j] / 100);
			int numberB = int(TotSizeLayer[i] * PortionB[j] / 100 * TestDefPort / 10);
			TMPres = FunctionAddNoiseAndDefence(NO, 1, layA, layB, layC, number, number, number, numberB);
			Result[NO] = TMPres;
			NO++;
		}
	}
	printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n");
	WriteToCSV(Result,NO);
	printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n"); printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n");
	
}




void mainV6()
{
	int TotSizeLayer[3] = { 784 * 200, 200 * 50, 50 * 10 };
	int PortionA[9] = { 1,2,3,4,5,6,7,8,9 };
	int PortionB[9] = { 1,2,3,4,5,6,7,8,9 };
	int PortionDef[9] = { 1,2,3,4,5,6,7,8,9 };
	int TestDefPort = 0;
	int T = 0;
	double ResultAt[200];
	double ResultDef[2000];
	double TMPres = 0;
	int NO = 0;
	int layA;
	int layB;
	int layC;
	//三个层
/*
	//WriteTOCSV();
	double Test[10] = { 1,2,3,4,5,6,7,8,9,0 };
	AddingNoise(Test, 5, 1, 10);
	for (int i = 0; i < 10; i++)
	{
		printf("==%f==,", Test[i]);
	}

	while (1)
	{
		;
	}

	*/

	for (int i = 0; i < 3; i++)
	{
		if (i == 0)
		{
			layA = 1;
			layB = 0;
			layC = 0;
		}
		if (i == 1)
		{
			layA = 0;
			layB = 1;
			layC = 0;
		}
		if (i == 2)
		{
			layA = 0;
			layB = 0;
			layC = 1;
		}
		//只攻击的
		/*
		for (int j = 0; j < 9; j++)
		{
			int number = int( TotSizeLayer[i] * PortionA[j] / 10);
			TMPres = FunctionAddNoise(NO, 0, layA, layB, layC, number, number, number);
			Result[NO] = TMPres;
			NO++;
		}
		for (int j = 0; j <9; j++)
		{
			int number = int(TotSizeLayer[i] * PortionB[j] / 100);
			TMPres = FunctionAddNoise(NO, 1, layA, layB, layC, number, number, number);
			Result[NO] = TMPres;
			NO++;
		}*/

		//只防御
		for (int j = 0; j < 9; j++)
		{
			int number = int(TotSizeLayer[i] * PortionA[j] / 10);
			for (T = 0; T < 9; T++)
			{
				int numberB = int(TotSizeLayer[i] * PortionA[j] / 10 * (T+1) / 10);
				TMPres = FunctionAddNoiseAndDefence(NO, 0, layA, layB, layC, number, number, number, numberB);
				ResultDef[NO] = TMPres;
				NO++;
			}
			for (T = 0; T < 9; T++)
			{
				int numberB = int(TotSizeLayer[i] * PortionA[j] / 100 * (T + 1) / 10);
				TMPres = FunctionAddNoiseAndDefence(NO, 0, layA, layB, layC, number, number, number, numberB);
				ResultDef[NO] = TMPres;
				NO++;
			}
		}
		for (int j = 0; j < 9; j++)
		{
			int number = int(TotSizeLayer[i] * PortionB[j] / 100);
			for (T = 0; T < 9; T++)
			{
				int numberB = int(TotSizeLayer[i] * PortionB[j] / 100 * (T + 1) / 10);
				TMPres = FunctionAddNoiseAndDefence(NO, 1, layA, layB, layC, number, number, number, numberB);
				ResultDef[NO] = TMPres;
				NO++;
			}
		}
	}
	WriteToCSV(ResultDef, NO);
	//WriteToCSVB(Result, NO);

}


//weight
double KSeond1WND[240 * 784];
double KThiredWND[140 * 240];
double KFourthWND[37 * 140];
//node
double KSeond1ND[240];
double KThiredND[140];
double KFourthND[37];
//bias
double KSeond1BND[240];
double KThiredBND[140];
double KFourthBND[37];

//save the model and weight. bias

void WriteWeightBais(int NO, int Type)
{
	char filenameWeight1[25] = "NWeight100.csv";

	char filenameWeight2[25] = "NWeight200.csv";

	char filenameWeight3[25] = "NWeight300.csv";

	char filenameWeight4[25] = "NBias100.csv";

	char filenameWeight5[25] = "NBias200.csv";

	char filenameWeight6[25] = "NBias300.csv";
	/*
	if (Type == 1)
	{
		filenameWeight1[0] = 'I';

		filenameWeight2[0] = 'I';

		filenameWeight3[0] = 'I';

		filenameWeight4[0] = 'I';

		filenameWeight5[0] = 'I';

		filenameWeight6[0] = 'I';


	}*/
	int Ten = int(NO / 10);
	int One = int(NO % 10);

	filenameWeight1[9] = One + '0';

	filenameWeight2[9] = One + '0';

	filenameWeight3[9] = One + '0';

	filenameWeight4[7] = One + '0';

	filenameWeight5[7] = One + '0';

	filenameWeight6[7] = One + '0';

	filenameWeight1[8] = Ten + '0';

	filenameWeight2[8] = Ten + '0';

	filenameWeight3[8] = Ten + '0';

	filenameWeight4[6] = Ten + '0';

	filenameWeight5[6] = Ten + '0';

	filenameWeight6[6] = Ten + '0';


	WriteToCSVB(NNode.ThiredB, 50, filenameWeight5);
	WriteToCSVB(NNode.Seond1B, 200, filenameWeight4);
	WriteToCSVB(NWeight.Fourth, 50 * 10, filenameWeight3);
	WriteToCSVB(NWeight.Thired, 200 * 50, filenameWeight2);
	WriteToCSVB(NWeight.Seond1, 200 * 784, filenameWeight1);
	WriteToCSVB(NNode.FourthB, 10, filenameWeight6);



	int count = 0;
	count = ReadWeightAndBiasFromFiles(filenameWeight1, KSeond1WND, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight2, KThiredWND, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight3, KFourthWND, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight4, KSeond1BND, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight5, KThiredBND, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight6, KFourthBND, 784, 200);
	int Total = 0;
	for (count = 0; count < 784; count++)
	{
		if (KSeond1WND[count] != NWeight.Seond1[count])
		{
			printf("original %f,    new is %f\n", NWeight.Seond1[count],KSeond1WND[count]);
			//printf("error found\n");
			Total++;
		}
	}
	printf("error found!   total:%d\n",Total);
	while (1)
	{
		;
	}
}

double ReadAndTest(int NO, int Type)
{
	char filenameWeight1[25] = "NWeight100.csv";

	char filenameWeight2[25] = "NWeight200.csv";

	char filenameWeight3[25] = "NWeight300.csv";

	char filenameWeight4[25] = "NBias100.csv";

	char filenameWeight5[25] = "NBias200.csv";

	char filenameWeight6[25] = "NBias300.csv";
	/*
	if (Type == 1)
	{
		filenameWeight1[0] = 'I';

		filenameWeight2[0] = 'I';

		filenameWeight3[0] = 'I';

		filenameWeight4[0] = 'I';

		filenameWeight5[0] = 'I';

		filenameWeight6[0] = 'I';


	}*/
	int Ten = int(NO / 10);
	int One = int(NO % 10);

	filenameWeight1[9] = One + '0';

	filenameWeight2[9] = One + '0';

	filenameWeight3[9] = One + '0';

	filenameWeight4[7] = One + '0';

	filenameWeight5[7] = One + '0';

	filenameWeight6[7] = One + '0';

	filenameWeight1[8] = Ten + '0';

	filenameWeight2[8] = Ten + '0';

	filenameWeight3[8] = Ten + '0';

	filenameWeight4[6] = Ten + '0';

	filenameWeight5[6] = Ten + '0';

	filenameWeight6[6] = Ten + '0';

	/*
	WriteToCSVB(NNode.ThiredB, 50, filenameWeight5);
	WriteToCSVB(NNode.Seond1B, 200, filenameWeight4);
	WriteToCSVB(NWeight.Fourth, 50 * 10, filenameWeight3);
	WriteToCSVB(NWeight.Thired, 200 * 50, filenameWeight2);
	WriteToCSVB(NWeight.Seond1, 200 * 784, filenameWeight1);
	WriteToCSVB(NNode.FourthB, 10, filenameWeight6);
*/
	int count = 0;
	printf("%s\n", filenameWeight1);
	printf("%s\n", filenameWeight2);
	printf("%s\n", filenameWeight3);
	printf("%s\n", filenameWeight4);
	printf("%s\n", filenameWeight5);
	printf("%s\n", filenameWeight6);
	count = ReadWeightAndBiasFromFiles(filenameWeight1, NWeight.Seond1, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight2, NWeight.Thired, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight3, NWeight.Fourth, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight4, NNode.Seond1B, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight5, NNode.ThiredB, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight6, NNode.FourthB, 784, 200);

	/*
	int k = 0;
	for (k = 0; k < 200; k++)
	{
		printf("======%d=====%f   \n", k,NWeight.Seond1[k]);
	}
	while (1)
	{
		;
	}
*/
	//=========================准备训练图片和测试图片的label==================
	char filenameTrainLabel[25] = "TrainingLabel.txt";
	char filenameTeestLabeel[25] = "TestingLabel.txt";
	int TrainLabel[60000];
	int TestLabel[10000];
	TReadImageMNistLabel(filenameTrainLabel, TrainLabel);
	TReadImageMNistLabel(filenameTeestLabeel, TestLabel);

	//=================准备训练图片和测试图片=================
	double imageB[784] = { 0 };
	//ReadImage(filename);
	//image=ReadImageMNist(filename);
	char filenameO[25] = "TrainingImage0.txt";
	char filenameB[25] = "TrainingImage0.txt";
	char filenameOT[25] = "TestingImage0.txt";
	char filenameBT[25] = "TestingImage0.txt";
	int countImages = 60000;
	int testImages = 10000;
	int countImageIndex = 1;

	int Pre_Res;
	int T;
	int kID;
	int AccCount = 0;
	int i, j;
	for (countImageIndex = 0; countImageIndex < testImages; countImageIndex++)
	{
		CopyFilename(filenameBT, filenameOT);
		nameMakerT(filenameBT, countImageIndex);
		//printf("obtain the file of: %s     \n", filenameB);
		TReadImageMNistC(filenameBT, imageB);
		/*
		for (kID = 0; kID < 784; kID++)
		{
			//printf("%d     %f\n", kID, imageB[kID]);
			printf("%d     %f\n", kID, NWeight.Seond1[kID]);

		}
		*/
		//==========forward propagation================
		initNode();
		FClayerForward(imageB, NNode.Seond1, NWeight.Seond1, 784, 200, NNode.Seond1B);
		FClayerForwardRelu(NNode.Seond1, 200);
		FClayerForward(NNode.Seond1, NNode.Thired, NWeight.Thired, 200, 50, NNode.ThiredB);
		FClayerForwardRelu(NNode.Thired, 50);
		FClayerForward(NNode.Thired, NNode.Fourth, NWeight.Fourth, 50, 10, NNode.FourthB);
		//==========calculate the loss deviation================
		//ONode.SumofExp = ExpSumToProb(NNode.Fourth, ONode.Exp , 10, ONode.Prob);
		Pre_Res = findMax(NNode.Fourth, 10);
		T = TestLabel[countImageIndex];//获取groundtruth

		if (Pre_Res == T)
		{
			AccCount++;
			//printf("=====%d   ---------%d\n", Pre_Res, T);
		}


	}
	double Acc = double(double(AccCount) / double(testImages));
	printf("The recognition accuracy on CPU is %f\n", Acc);
	return Acc;
}


void mainK11()
{
	int i = 0;
	double Res = 0;
	double ResAll[54];
	for (i = 0; i < 54; i++)
	{
		Res = ReadAndTest(i, 1);
		ResAll[i] = Res;
		printf("Acc is %f \n", Res);
	}

	printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n");
	char FIleName[25] = "DynResult.csv";
	WriteToCSVB(ResAll, 54,FIleName);
	printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n"); printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n");
}

/* V2, fault injection with multiple test and collect the result
Using our method for defence.
*/

double FunctionAddNoiseAndDefenceV2(int NO, int Type, int layA, int layB, int layC, int numberA, int numberB, int numberC, int DefLeng,int ProLen)
{
	/*
		NO: 总测试次数
		Type: 0:随机添加， 1 最重要的添加
		layA,layB,layC 对三层网络节点添加的标志
		numberA,B,C 添加数目。
	*/
	printf("Test using existing weight!\n");

	char WeightWA[25] = "001_WA_F.csv";
	char WeightWB[25] = "001_WB_F.csv";
	char WeightWC[25] = "001_WC_F.csv";
	char WeightBAC[25] = "001_BA_F.csv";
	char WeightBBC[25] = "001_BB_F.csv";
	char WeightBCC[25] = "001_BC_F.csv";
	int count = 0;
	count = ReadWeightAndBiasFromFiles(WeightWA, NWeight.Seond1, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightWB, NWeight.Thired, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightWC, NWeight.Fourth, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBAC, NNode.Seond1B, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBBC, NNode.ThiredB, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBCC, NNode.FourthB, 784, 200);


	/*============================Start====================================
	Protection:
	1.random
	2.adding noise
	==============================Start===============================*/
	printf("\n");
	printf("======%d====adding noise==%d=======\n", NO, numberA);
	printf("\n");
	int NumbOfNoise = 100;
	int AddingType = Type;   //0 randomly, 1->important first
	int Welen = 200 ;
	if (layA)
	{
		Welen = 200* ProLen/10;
		AddingNoiseOurDef(NWeight.Seond1, numberA, AddingType, 784,200, Welen, DefLeng);
		//WriteToCSV(NWeight.Seond1, 784*200);
		//return 0;
	}
	if (layB)
	{
		Welen =  50 * ProLen / 10;
		AddingNoiseOurDef(NWeight.Thired, numberB, AddingType, 200, 50, Welen, DefLeng);
	}
	if (layC)
	{
		Welen = 10 * ProLen / 10;
		AddingNoiseOurDef(NWeight.Fourth, numberC, AddingType, 50,10, Welen,  DefLeng);
	}
	//NWeight.Fourth;

	WriteWeightBais(NO, Type);
	return 0;
	printf("==========adding noise  done===\n");
	//=========================准备训练图片和测试图片的label==================
	char filenameTrainLabel[25] = "TrainingLabel.txt";
	char filenameTeestLabeel[25] = "TestingLabel.txt";
	int TrainLabel[60000];
	int TestLabel[10000];
	TReadImageMNistLabel(filenameTrainLabel, TrainLabel);
	TReadImageMNistLabel(filenameTeestLabeel, TestLabel);

	//=================准备训练图片和测试图片=================
	double imageB[784] = { 0 };
	//ReadImage(filename);
	//image=ReadImageMNist(filename);
	char filenameO[25] = "TrainingImage0.txt";
	char filenameB[25] = "TrainingImage0.txt";
	char filenameOT[25] = "TestingImage0.txt";
	char filenameBT[25] = "TestingImage0.txt";
	int countImages = 60000;
	int testImages = 10000;
	int countImageIndex = 1;

	int Pre_Res;
	int T;
	int kID;
	int AccCount = 0;
	int i, j;
	for (countImageIndex = 0; countImageIndex < testImages; countImageIndex++)
	{
		CopyFilename(filenameBT, filenameOT);
		nameMakerT(filenameBT, countImageIndex);
		//printf("obtain the file of: %s     \n", filenameB);
		TReadImageMNistC(filenameBT, imageB);
		/*
		for (kID = 0; kID < 784; kID++)
		{
			//printf("%d     %f\n", kID, imageB[kID]);
			printf("%d     %f\n", kID, NWeight.Seond1[kID]);

		}
		*/
		//==========forward propagation================
		initNode();
		FClayerForward(imageB, NNode.Seond1, NWeight.Seond1, 784, 200, NNode.Seond1B);
		FClayerForwardRelu(NNode.Seond1, 200);
		FClayerForward(NNode.Seond1, NNode.Thired, NWeight.Thired, 200, 50, NNode.ThiredB);
		FClayerForwardRelu(NNode.Thired, 50);
		FClayerForward(NNode.Thired, NNode.Fourth, NWeight.Fourth, 50, 10, NNode.FourthB);
		//==========calculate the loss deviation================
		//ONode.SumofExp = ExpSumToProb(NNode.Fourth, ONode.Exp , 10, ONode.Prob);
		Pre_Res = findMax(NNode.Fourth, 10);
		T = TestLabel[countImageIndex];//获取groundtruth

		if (Pre_Res == T)
		{
			AccCount++;
			//printf("=====%d   ---------%d\n", Pre_Res, T);
		}


	}
	double Acc = double(double(AccCount) / double(testImages));
	printf("The recognition accuracy on CPU is %f\n", Acc);
	return Acc;

}


/*
mainV8: 实现了基本功能，但是每次测试只针对每一层
*/

void mainV8()

{
	printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n");
	int TotSizeLayer[3] = { 784 * 200, 200 * 50, 50 * 10 };
	int PortionA[9] = { 1,2,3,4,5,6,7,8,9 };
	int PortionB[9] = { 1,2,3,4,5,6,7,8,9 };
	int PortionDef[9] = { 1,2,3,4,5,6,7,8,9 };
	int TestDefPort = 5;
	double Result[1000];
	double TMPres = 0;
	int Prolen =2;
	int NO = 0;
	int layA;
	int layB;
	int layC;
	//三个层
/*
	//WriteTOCSV();
	double Test[10] = { 1,2,3,4,5,6,7,8,9,0 };
	AddingNoise(Test, 5, 1, 10);
	for (int i = 0; i < 10; i++)
	{
		printf("==%f==,", Test[i]);
	}

	while (1)
	{
		;
	}

	*/

	for (int i = 0; i < 3; i++)
	{
		if (i == 0)
		{
			layA = 1;
			layB = 0;
			layC = 0;
		}
		if (i == 1)
		{
			layA = 0;
			layB = 1;
			layC = 0;
			//Prolen = Prolen + 1;
		}
		if (i == 2)
		{
			layA = 0;
			layB = 0;
			layC = 1;
			Prolen = Prolen + 4;
		}
		//只攻击的
		/*
		for (int j = 0; j < 9; j++)
		{
			int number = int( TotSizeLayer[i] * PortionA[j] / 10);
			TMPres = FunctionAddNoise(NO, 0, layA, layB, layC, number, number, number);
			Result[NO] = TMPres;
			NO++;
		}
		for (int j = 0; j <9; j++)
		{
			int number = int(TotSizeLayer[i] * PortionB[j] / 100);
			TMPres = FunctionAddNoise(NO, 1, layA, layB, layC, number, number, number);
			Result[NO] = TMPres;
			NO++;
		}
*/
//只防御
		
		for (int j = 0; j < 9; j++)
		{
			int number = int(TotSizeLayer[i] * PortionA[j] / 10);
			int numberB = int(TotSizeLayer[i] * PortionA[j] / 10 * TestDefPort / 10);
			printf(" >>>>>>>>>>>>>>>>>>>>>>>>checking numbers %d     ====    %d\n", number, numberB);
			TMPres = FunctionAddNoiseAndDefenceV2(NO, 0, layA, layB, layC, number, number, number, numberB, Prolen);
			Result[NO] = TMPres;
			NO++;
		}
		for (int j = 0; j < 9; j++)
		{
			int number = int(TotSizeLayer[i] * PortionB[j] / 100);
			int numberB = int(TotSizeLayer[i] * PortionB[j] / 100 * TestDefPort / 10);
			TMPres = FunctionAddNoiseAndDefenceV2(NO, 1, layA, layB, layC, number, number, number, numberB, Prolen);
			
			Result[NO] = TMPres;
			NO++;
		}
		//break;
	}
	//break;
	printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n");
	//WriteToCSV(Result, NO);
	printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n"); printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n");

}





/* V3, All fault injection on 3 layers with multiple test and collect the result
Using our method for defence.
*/

double FunctionAddNoiseAndDefenceV3(int NO, int Type, int layA, int layB, int layC, int numberA, int numberB, int numberC, int DefLeng, int ProLen)
{
	/*
		NO: 总测试次数
		Type: 0:随机添加， 1 最重要的添加
		layA,layB,layC 对三层网络节点添加的标志
		numberA,B,C 添加数目。
	*/
	printf("Test using existing weight!\n");

	char WeightWA[25] = "001_WA_F.csv";
	char WeightWB[25] = "001_WB_F.csv";
	char WeightWC[25] = "001_WC_F.csv";
	char WeightBAC[25] = "001_BA_F.csv";
	char WeightBBC[25] = "001_BB_F.csv";
	char WeightBCC[25] = "001_BC_F.csv";
	int count = 0;
	count = ReadWeightAndBiasFromFiles(WeightWA, NWeight.Seond1, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightWB, NWeight.Thired, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightWC, NWeight.Fourth, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBAC, NNode.Seond1B, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBBC, NNode.ThiredB, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBCC, NNode.FourthB, 784, 200);


	/*============================Start====================================
	Protection:
	1.random
	2.adding noise
	==============================Start===============================*/
	printf("\n");
	printf("======%d====adding noise==%d=======\n", NO, numberA);
	printf("\n");
	int NumbOfNoise = 100;
	int AddingType = Type;   //0 randomly, 1->important first
	int Welen = 200;
	if (layA)
	{
		Welen = 200 * ProLen / 10;
		AddingNoiseOurDef(NWeight.Seond1, numberA, AddingType, 784, 200, Welen, DefLeng);
	}
	if (layB)
	{
		Welen = 50 * ProLen / 10;
		AddingNoiseOurDef(NWeight.Thired, numberB, AddingType, 200, 50, Welen, DefLeng);
	}
	if (layC)
	{
		//Welen = 10 * (ProLen+4) / 10;
		Welen = 10 * (ProLen + 4) / 10;
		AddingNoiseOurDef(NWeight.Fourth, numberC, AddingType, 50, 10, Welen, DefLeng);
	}
	//NWeight.Fourth;



	printf("==========adding noise  done===\n");
	//=========================准备训练图片和测试图片的label==================
	char filenameTrainLabel[25] = "TrainingLabel.txt";
	char filenameTeestLabeel[25] = "TestingLabel.txt";
	int TrainLabel[60000];
	int TestLabel[10000];
	TReadImageMNistLabel(filenameTrainLabel, TrainLabel);
	TReadImageMNistLabel(filenameTeestLabeel, TestLabel);

	//=================准备训练图片和测试图片=================
	double imageB[784] = { 0 };
	//ReadImage(filename);
	//image=ReadImageMNist(filename);
	char filenameO[25] = "TrainingImage0.txt";
	char filenameB[25] = "TrainingImage0.txt";
	char filenameOT[25] = "TestingImage0.txt";
	char filenameBT[25] = "TestingImage0.txt";
	int countImages = 60000;
	int testImages = 10000;
	int countImageIndex = 1;

	int Pre_Res;
	int T;
	int kID;
	int AccCount = 0;
	int i, j;
	for (countImageIndex = 0; countImageIndex < testImages; countImageIndex++)
	{
		CopyFilename(filenameBT, filenameOT);
		nameMakerT(filenameBT, countImageIndex);
		//printf("obtain the file of: %s     \n", filenameB);
		TReadImageMNistC(filenameBT, imageB);
		/*
		for (kID = 0; kID < 784; kID++)
		{
			//printf("%d     %f\n", kID, imageB[kID]);
			printf("%d     %f\n", kID, NWeight.Seond1[kID]);

		}
		*/
		//==========forward propagation================
		initNode();
		FClayerForward(imageB, NNode.Seond1, NWeight.Seond1, 784, 200, NNode.Seond1B);
		FClayerForwardRelu(NNode.Seond1, 200);
		FClayerForward(NNode.Seond1, NNode.Thired, NWeight.Thired, 200, 50, NNode.ThiredB);
		FClayerForwardRelu(NNode.Thired, 50);
		FClayerForward(NNode.Thired, NNode.Fourth, NWeight.Fourth, 50, 10, NNode.FourthB);
		//==========calculate the loss deviation================
		//ONode.SumofExp = ExpSumToProb(NNode.Fourth, ONode.Exp , 10, ONode.Prob);
		Pre_Res = findMax(NNode.Fourth, 10);
		T = TestLabel[countImageIndex];//获取groundtruth

		if (Pre_Res == T)
		{
			AccCount++;
			//printf("=====%d   ---------%d\n", Pre_Res, T);
		}


	}
	double Acc = double(double(AccCount) / double(testImages));
	printf("The recognition accuracy on CPU is %f\n", Acc);
	return Acc;

}


/*Final_Before I add FPGA*/
void mainV9()

{
	printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n");
	int TotSizeLayer[3] = { 784 * 200, 200 * 50, 50 * 10 };
	int PortionA[9] = { 1,2,3,4,5,6,7,8,9 };
	int PortionB[9] = { 1,2,3,4,5,6,7,8,9 };
	int PortionDef[9] = { 1,2,3,4,5,6,7,8,9 };
	int TestDefPort = 5;
	double Result[1000];
	double TMPres = 0;
	int Prolen = 3;
	int NO = 0;
	int layA;
	int layB;
	int layC;
	//三个层
/*
	//WriteTOCSV();
	double Test[10] = { 1,2,3,4,5,6,7,8,9,0 };
	AddingNoise(Test, 5, 1, 10);
	for (int i = 0; i < 10; i++)
	{
		printf("==%f==,", Test[i]);
	}

	while (1)
	{
		;
	}

	*/

		
	layA = 1;
	layB = 1;
	layC = 1;
		
	//只攻击的
	/*
	for (int j = 0; j < 9; j++)
	{
		int number = int( TotSizeLayer[i] * PortionA[j] / 10);
		TMPres = FunctionAddNoise(NO, 0, layA, layB, layC, number, number, number);
		Result[NO] = TMPres;
		NO++;
	}
	for (int j = 0; j <9; j++)
	{
		int number = int(TotSizeLayer[i] * PortionB[j] / 100);
		TMPres = FunctionAddNoise(NO, 1, layA, layB, layC, number, number, number);
		Result[NO] = TMPres;
		NO++;
	}*/
	/*
	for (int j = 0; j < 9; j++)
	{
		int numbera = int(TotSizeLayer[0] * PortionA[j] / 10);
		int numberb = int(TotSizeLayer[1] * PortionA[j] / 10);
		int numberc = int(TotSizeLayer[2] * PortionA[j] / 10);
		int numberB = TestDefPort;
		TMPres = FunctionAddNoise(NO, 0, layA, layB, layC, numbera, numberb, numberc);
		Result[NO] = TMPres;
		NO++;
	}
	for (int j = 0; j < 9; j++)
	{
		int numbera = int(TotSizeLayer[0] * PortionA[j] / 100);
		int numberb = int(TotSizeLayer[1] * PortionA[j] / 100);
		int numberc = int(TotSizeLayer[2] * PortionA[j] / 100);
		int numberB = TestDefPort;
		TMPres = FunctionAddNoise(NO, 1, layA, layB, layC, numbera, numberb, numberc);
		Result[NO] = TMPres;
		NO++;
	}
*/

//只防御
	for (int j = 0; j < 9; j++)
	{
		int numbera = int(TotSizeLayer[0] * PortionA[j] / 10);
		int numberb = int(TotSizeLayer[1] * PortionA[j] / 10);
		int numberc = int(TotSizeLayer[2] * PortionA[j] / 10);
		int numberB = TestDefPort;
		TMPres = FunctionAddNoiseAndDefenceV3(NO, 0, layA, layB, layC, numbera, numberb, numberc, numberB, Prolen);
		Result[NO] = TMPres;
		NO++;
	}
	for (int j = 0; j < 9; j++)
	{
		int numbera = int(TotSizeLayer[0] * PortionA[j] / 100);
		int numberb = int(TotSizeLayer[1] * PortionA[j] / 100);
		int numberc = int(TotSizeLayer[2] * PortionA[j] / 100);
		int numberB = TestDefPort;
		TMPres = FunctionAddNoiseAndDefenceV3(NO, 1, layA, layB, layC, numbera, numberb, numberc, numberB, Prolen);
		Result[NO] = TMPres;
		NO++;
	}
	
	printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n");
	WriteToCSV(Result, NO);
	printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n"); printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n");

}




/* ==========================================================================================================
	last task: check the recognition accuracy in the two protection methods.


*/



//====================================================================================modify weight =================================================== 8

/*new weight for fixed portion checking================================================================*/
//weight
int DemANF = 260;
int DemBNF = 185;
int DemCNF = 44;
double Seond1WNF[260 * 784];
double ThiredWNF[185 * 260];
double FourthWNF[44 * 185];
//node
double Seond1NF[260];
double ThiredNF[185];
double FourthNF[44];
//bias
double Seond1BNF[260];
double ThiredBNF[185];
double FourthBNF[44];

void ModifyWeightFix(int *Pair1, int *Pair2, int *Pair3)
{
	int i = 0;
	int j = 0;
	//weight 1
	for (j = 0; j < 784; j++)
	{
		for (i = 0; i < 260; i++)
		{
			if (i < 200)
			{
				Seond1WNF[j * 260 + i] = NWeight.Seond1[j * 200 + i];
			}
			else
			{
				Seond1WNF[j * 260 + i] = NWeight.Seond1[j * 200 + Pair1[i - 200]];
			}

		}
	}
	//weight 2
	for (j = 0; j < 260; j++)
	{
		if (j < 200)
		{
			for (i = 0; i < 185; i++)
			{
				if (i < 50)
				{
					ThiredWNF[j * 185 + i] = NWeight.Thired[j * 50 + i];
				}
				else
				{
					if (i < 60)
					{
						ThiredWNF[j * 185 + i] = NWeight.Thired[j * 50 + Pair2[i - 50]];
					}
					else
					{
						ThiredWNF[j * 185 + i] = 1;
					}

				}

			}
		}
		else
		{
			for (i = 0; i < 185; i++)
			{
				ThiredWNF[j * 185 + i] = 0;
			}
		}

	}
	//weight 3
	for (j = 0; j < 185; j++)
	{
		if (j < 50)
		{
			for (i = 0; i < 44; i++)
			{
				if (i < 10)
				{
					FourthWNF[j * 44 + i] = NWeight.Fourth[j * 10 + i];
				}
				else
				{
					if (i < 13)
					{
						FourthWNF[j * 44 + i] = NWeight.Fourth[j * 10 + Pair3[i - 10]];
					}
					else
					{
						FourthWNF[j * 44 + i] = 1;
					}

				}

			}
		}
		else
		{
			for (i = 0; i < 44; i++)
			{
				FourthWNF[j * 44 + i] = 0;
			}
		}

	}
	for (j = 0; j < 260; j++)
	{
		Seond1NF[j] = 0.0;
		if (j < 200)
		{
			Seond1BNF[j] = NNode.Seond1B[j];
		}
		else
		{
			Seond1BNF[j] = 0;
		}
	}
	for (j = 0; j < 185; j++)
	{
		ThiredNF[j] = 0.0;
		if (j < 50)
		{
			ThiredBNF[j] = NNode.ThiredB[j];
		}
		else
		{
			ThiredBNF[j] = 0;
		}
	}
	for (j = 0; j < 44; j++)
	{
		FourthNF[j] = 0.0;
		if (j < 10)
		{
			FourthBNF[j] = NNode.FourthB[j];
		}
		else
		{
			FourthBNF[j] = 0;
		}
	}
}



/*new weight for dynamic portion checking==============================================================================*/
int DemAND = 240;
int DemBND = 140;
int DemCND = 37;
//weight
double Seond1WND[240 * 784];
double ThiredWND[140 * 240];
double FourthWND[37 * 140];
//node
double Seond1ND[240];
double ThiredND[140];
double FourthND[37];
//bias
double Seond1BND[240];
double ThiredBND[140];
double FourthBND[37];

void ModifyWeightDyn(int *Pair1, int *Pair2, int *Pair3)
{
	int i = 0;
	int j = 0;
	//weight 1
	for (j = 0; j < 784; j++)
	{
		for (i = 0; i < 240; i++)
		{
			if (i < 200)
			{
				Seond1WND[j * 240 + i] = NWeight.Seond1[j * 200 + i];
			}
			else
			{
				Seond1WND[j * 240 + i] = NWeight.Seond1[j * 200 + Pair1[i - 200]];
			}

		}
	}
	//weight 2
	for (j = 0; j < 240; j++)
	{
		if (j < 200)
		{
			for (i = 0; i < 140; i++)
			{
				if (i < 50)
				{
					ThiredWND[j * 140 + i] = NWeight.Thired[j * 50 + i];
				}
				else
				{
					if (i < 80)
					{
						ThiredWND[j * 140 + i] = NWeight.Thired[j * 50 + Pair2[i - 50]];
					}
					else
					{
						ThiredWND[j * 140 + i] = 1;
					}

				}

			}
		}
		else
		{
			for (i = 0; i < 140; i++)
			{
				ThiredWND[j * 140 + i] = 0;
			}
		}

	}
	//weight 3
	for (j = 0; j < 140; j++)
	{
		if (j < 50)
		{
			for (i = 0; i < 37; i++)
			{
				if (i < 10)
				{
					FourthWND[j * 37 + i] = NWeight.Fourth[j * 10 + i];
				}
				else
				{
					if (i < 16)
					{
						FourthWND[j * 37 + i] = NWeight.Fourth[j * 10 + Pair3[i - 10]];
					}
					else
					{
						FourthWND[j * 37 + i] = 1;
					}

				}

			}
		}
		else
		{
			for (i = 0; i < 37; i++)
			{
				FourthWND[j * 37 + i] = 0;
			}
		}

	}
	for (j = 0; j < 240; j++)
	{
		Seond1ND[j] = 0.0;
		if (j < 200)
		{
			Seond1BND[j] = NNode.Seond1B[j];
		}
		else
		{
			Seond1BND[j] = 0;
		}
	}
	for (j = 0; j < 140; j++)
	{
		ThiredND[j] = 0.0;
		if (j < 50)
		{
			ThiredBND[j] = NNode.ThiredB[j];
		}
		else
		{
			ThiredBND[j] = 0;
		}
	}
	for (j = 0; j < 37; j++)
	{
		FourthND[j] = 0.0;
		if (j < 10)
		{
			FourthBND[j] = NNode.FourthB[j];
		}
		else
		{
			FourthBND[j] = 0;
		}
	}

}

void ModifyWeightFixB(int *Pair1, int *Pair2, int *Pair3)
{
	int i = 0;
	int j = 0;
	//weight 1
	for (j = 0; j < 784; j++)
	{
		for (i = 0; i < 240; i++)
		{
			if (i < 200)
			{
				Seond1WND[j * 240 + i] = NWeight.Seond1[j * 200 + i];
			}
			else
			{
				Seond1WND[j * 240 + i] = NWeight.Seond1[j * 200 + Pair1[i - 200]];
			}

		}
	}
	//weight 2
	for (j = 0; j < 240; j++)
	{
		if (j < 200)
		{
			for (i = 0; i < 140; i++)
			{
				if (i < 50)
				{
					ThiredWND[j * 140 + i] = NWeight.Thired[j * 50 + i];
				}
				else
				{
					if (i < 80)
					{
						ThiredWND[j * 140 + i] = NWeight.Thired[j * 50 + Pair2[i - 50]];
					}
					else
					{
						ThiredWND[j * 140 + i] = 1;
					}

				}

			}
		}
		else
		{
			for (i = 0; i < 140; i++)
			{
				ThiredWND[j * 140 + i] = 0;
			}
		}

	}
	//weight 3
	for (j = 0; j < 140; j++)
	{
		if (j < 50)
		{
			for (i = 0; i < 37; i++)
			{
				if (i < 10)
				{
					FourthWND[j * 37 + i] = NWeight.Fourth[j * 10 + i];
				}
				else
				{
					if (i < 16)
					{
						FourthWND[j * 37 + i] = NWeight.Fourth[j * 10 + Pair3[i - 10]];
					}
					else
					{
						FourthWND[j * 37 + i] = 1;
					}

				}

			}
		}
		else
		{
			for (i = 0; i < 37; i++)
			{
				FourthWND[j * 37 + i] = 0;
			}
		}

	}
	for (j = 0; j < 240; j++)
	{
		Seond1ND[j] = 0.0;
		if (j < 200)
		{
			Seond1BND[j] = NNode.Seond1B[j];
		}
		else
		{
			Seond1BND[j] = 0;
		}
	}
	for (j = 0; j < 140; j++)
	{
		ThiredND[j] = 0.0;
		if (j < 50)
		{
			ThiredBND[j] = NNode.ThiredB[j];
		}
		else
		{
			ThiredBND[j] = 0;
		}
	}
	for (j = 0; j < 37; j++)
	{
		FourthND[j] = 0.0;
		if (j < 10)
		{
			FourthBND[j] = NNode.FourthB[j];
		}
		else
		{
			FourthBND[j] = 0;
		}
	}
}


void initNodeK()
{
	int j = 0;
	for (j = 0; j < 240; j++)
	{
		Seond1ND[j] = 0.0;

	}
	for (j = 0; j < 140; j++)
	{
		ThiredND[j] = 0.0;

	}
	for (j = 0; j < 37; j++)
	{
		FourthND[j] = 0.0;

	}
	for (j = 0; j < 260; j++)
	{
		Seond1NF[j] = 0.0;

	}
	for (j = 0; j < 185; j++)
	{
		ThiredNF[j] = 0.0;

	}
	for (j = 0; j < 44; j++)
	{
		FourthNF[j] = 0.0;
	}
}
//====================================================================================modify weight end===================================================8


double FunctionCheckPerformance(int demA ,int demB, int demC)
{
	/*
		demA.b.c 三个维度。
	*/
	printf("Test using modified weight!\n");
	
	//printf("==========adding noise  done===\n");
	//=========================准备训练图片和测试图片的label==================
	char filenameTrainLabel[25] = "TrainingLabel.txt";
	char filenameTeestLabeel[25] = "TestingLabel.txt";
	int TrainLabel[60000];
	int TestLabel[10000];
	TReadImageMNistLabel(filenameTrainLabel, TrainLabel);
	TReadImageMNistLabel(filenameTeestLabeel, TestLabel);

	//=================准备训练图片和测试图片=================
	double imageB[784] = { 0 };
	//ReadImage(filename);
	//image=ReadImageMNist(filename);
	char filenameO[25] = "TrainingImage0.txt";
	char filenameB[25] = "TrainingImage0.txt";
	char filenameOT[25] = "TestingImage0.txt";
	char filenameBT[25] = "TestingImage0.txt";
	int countImages = 60000;
	int testImages = 10000;
	int countImageIndex = 1;

	int Pre_Res;
	int T;
	int kID;
	int AccCount = 0;
	int i, j;
	for (countImageIndex = 0; countImageIndex < testImages; countImageIndex++)
	{
		CopyFilename(filenameBT, filenameOT);
		nameMakerT(filenameBT, countImageIndex);
		//printf("obtain the file of: %s     \n", filenameB);
		TReadImageMNistC(filenameBT, imageB);
		/*
		for (kID = 0; kID < 784; kID++)
		{
			//printf("%d     %f\n", kID, imageB[kID]);
			printf("%d     %f\n", kID, NWeight.Seond1[kID]);

		}

		double Seond1WNF[260 * 784];
double ThiredWNF[185 * 260];
double FourthWNF[44 * 185];
//node
double Seond1NF[260];
double ThiredNF[185];
double FourthNF[44];
//bias
double Seond1BNF[260];
double ThiredBNF[185];
double FourthBNF[44];

		*/
		//==========forward propagation================
		if (demA == 260)
		{
			initNodeK();
			FClayerForward(imageB, Seond1NF, Seond1WNF, 784, demA, Seond1BNF);
			FClayerForwardRelu(Seond1NF, demA);
			FClayerForward(Seond1NF, ThiredNF, ThiredWNF, demA, demB, ThiredBNF);
			FClayerForwardRelu(ThiredNF, demB);
			FClayerForward(ThiredNF, FourthNF, FourthWNF, demB, demC, FourthBNF);
		}
		else
		{
			initNodeK();
			FClayerForward(imageB, Seond1ND, Seond1WND, 784, demA, Seond1BND);
			FClayerForwardRelu(Seond1ND, demA);
			FClayerForward(Seond1ND, ThiredND, ThiredWND, demA, demB, ThiredBND);
			FClayerForwardRelu(ThiredND, demB);
			FClayerForward(ThiredND, FourthND, FourthWND, demB, demC, FourthBND);
		}
		
		//==========calculate the loss deviation================
		//ONode.SumofExp = ExpSumToProb(NNode.Fourth, ONode.Exp , 10, ONode.Prob);
		if (demA == 260)
		{
			Pre_Res = findMax(FourthNF, 10);
		}
		else
		{
			Pre_Res = findMax(FourthND, 10);
		}
		T = TestLabel[countImageIndex];//获取groundtruth

		if (Pre_Res == T)
		{
			AccCount++;
			//printf("=====%d   ---------%d\n", Pre_Res, T);
		}


	}
	double Acc = double(double(AccCount) / double(testImages));
	printf("The recognition accuracy on CPU is %f\n", Acc);
	return Acc;

}

//存新权重 ： 动态与静态模型权重
void mainV11()
{
	int Parir1[100];
	int Parir2[100];
	int Parir3[100];
	int i = 0;
	for (i = 0; i < 100; i++)
	{
		Parir1[i] = i;
		Parir2[i] = i;
		Parir3[i] = i;
	}


	//read original weight

	char WeightWA[25] = "001_WA_F.csv";
	char WeightWB[25] = "001_WB_F.csv";
	char WeightWC[25] = "001_WC_F.csv";
	char WeightBAC[25] = "001_BA_F.csv";
	char WeightBBC[25] = "001_BB_F.csv";
	char WeightBCC[25] = "001_BC_F.csv";
	int count = 0;
	count = ReadWeightAndBiasFromFiles(WeightWA, NWeight.Seond1, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightWB, NWeight.Thired, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightWC, NWeight.Fourth, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBAC, NNode.Seond1B, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBBC, NNode.ThiredB, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBCC, NNode.FourthB, 784, 200);
	double Result = 0;
	int DemA = 260;
	int DemB = 185;
	int DemC = 44;
	ModifyWeightFix(Parir1, Parir2, Parir3);
	Result=FunctionCheckPerformance(DemA, DemB, DemC);
	printf("the result is %f \n", Result);
	DemA = 240;
	DemB = 140;
	DemC = 37;
	ModifyWeightDyn(Parir1, Parir2, Parir3);
	//ModifyWeightFixB(Parir1, Parir2, Parir3);
	Result = FunctionCheckPerformance(DemA, DemB, DemC);
	printf("the result is %f \n", Result);
	/*写文件 权重 偏置*/

	printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n");
	char filenameWeight1[25] = "FixWeight1.csv";
	WriteToCSVB(Seond1WNF, 260 * 784,filenameWeight1);
	char filenameWeight2[25] = "FixWeight2.csv";
	WriteToCSVB(ThiredWNF, 260 * 185, filenameWeight2);
	char filenameWeight3[25] = "FixWeight3.csv";
	WriteToCSVB(FourthWNF, 44 * 185, filenameWeight3);
	char filenameWeight4[25] = "FixBias1.csv";
	WriteToCSVB(Seond1BNF, 260 , filenameWeight4);
	char filenameWeight5[25] = "FixBias2.csv";
	WriteToCSVB(ThiredBNF, 185 , filenameWeight5);
	char filenameWeight6[25] = "FixBias3.csv";
	WriteToCSVB(FourthBNF, 44 , filenameWeight6);


	char filenameWeight1d[25] = "DynWeight1.csv";
	WriteToCSVB(Seond1WND, 240 * 784, filenameWeight1d);
	char filenameWeight2d[25] = "DynWeight2.csv";
	WriteToCSVB(ThiredWND, 240 * 140, filenameWeight2d);
	char filenameWeight3d[25] = "DynWeight3.csv";
	WriteToCSVB(FourthWND, 37 * 140, filenameWeight3d);
	char filenameWeight4d[25] = "DynBias1.csv";
	WriteToCSVB(Seond1BND, 240, filenameWeight4d);
	char filenameWeight5d[25] = "DynBias2.csv";
	WriteToCSVB(ThiredBND, 140, filenameWeight5d);
	char filenameWeight6d[25] = "DynBias3.csv";
	WriteToCSVB(FourthBND, 37, filenameWeight6d);


	printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n"); printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n");

	while (1)
	{
		;
	}
}


//测试新权重 ： 动态与静态模型权重
void mainV12()
{
	int count;
/*
	char WeightWA[25] = "001_WA_F.csv";
	char WeightWB[25] = "001_WB_F.csv";
	char WeightWC[25] = "001_WC_F.csv";
	char WeightBAC[25] = "001_BA_F.csv";
	char WeightBBC[25] = "001_BB_F.csv";
	char WeightBCC[25] = "001_BC_F.csv";
	//int count = 0;
	
	count = ReadWeightAndBiasFromFiles(WeightWA, NWeight.Seond1, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightWB, NWeight.Thired, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightWC, NWeight.Fourth, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBAC, NNode.Seond1B, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBBC, NNode.ThiredB, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBCC, NNode.FourthB, 784, 200);
	*/
	/*读文件 权重 偏置*/

	printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n");
	char filenameWeight1[25] = "FixWeight1.csv";
	//WriteToCSVB(Seond1WNF, 260 * 784, filenameWeight1);
	char filenameWeight2[25] = "FixWeight2.csv";
	//WriteToCSVB(ThiredWNF, 260 * 185, filenameWeight2);
	char filenameWeight3[25] = "FixWeight3.csv";
	//WriteToCSVB(FourthWNF, 44 * 185, filenameWeight3);
	char filenameWeight4[25] = "FixBias1.csv";
	//WriteToCSVB(Seond1BNF, 260, filenameWeight4);
	char filenameWeight5[25] = "FixBias2.csv";
	//WriteToCSVB(ThiredBNF, 185, filenameWeight5);
	char filenameWeight6[25] = "FixBias3.csv";
	//WriteToCSVB(FourthBNF, 44, filenameWeight6);
	count = ReadWeightAndBiasFromFiles(filenameWeight6, FourthBNF, 784, 200);
	for (count = 0; count < 44; count++)
	{
		printf("%f\n", FourthBNF[count]);
	}

	for (count = 0; count < 10; count++)
	{
		printf("-----%f\n", NNode.FourthB[count]);
	}
	char Numb[4] = "0";
	double T = CalculateNumber(Numb);
	printf("=====%f \n", T);

	
	count = ReadWeightAndBiasFromFiles(filenameWeight1, Seond1WNF, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight2, ThiredWNF, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight3, FourthWNF, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight4, Seond1BNF, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight5, ThiredBNF, 784, 200);
	//count = ReadWeightAndBiasFromFiles(filenameWeight6, FourthBNF, 784, 200);

	char filenameWeight1d[25] = "DynWeight1.csv";
	//WriteToCSVB(Seond1WND, 260 * 784, filenameWeight1d);
	char filenameWeight2d[25] = "DynWeight2.csv";
	//WriteToCSVB(ThiredWND, 260 * 185, filenameWeight2d);
	char filenameWeight3d[25] = "DynWeight3.csv";
	//WriteToCSVB(FourthWND, 44 * 185, filenameWeight3d);
	char filenameWeight4d[25] = "DynBias1.csv";
	//WriteToCSVB(Seond1BND, 260, filenameWeight4d);
	char filenameWeight5d[25] = "DynBias2.csv";
	//WriteToCSVB(ThiredBND, 185, filenameWeight5d);
	char filenameWeight6d[25] = "DynBias3.csv";
	//WriteToCSVB(FourthBND, 44, filenameWeight6d);

	count = ReadWeightAndBiasFromFiles(filenameWeight1d, Seond1WND, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight2d, ThiredWND, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight3d, FourthWND, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight4d, Seond1BND, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight5d, ThiredBND, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight6d, FourthBND, 784, 200);

	printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n"); 
	
	double Result = 0;
	int DemA = 260;
	int DemB = 185;
	int DemC = 44;
	//ModifyWeightFix(Parir1, Parir2, Parir3);
	Result = FunctionCheckPerformance(DemA, DemB, DemC);
	printf("the result is %f \n", Result);
	DemA = 240;
	DemB = 140;
	DemC = 37;
	//ModifyWeightDyn(Parir1, Parir2, Parir3);
	//ModifyWeightFixB(Parir1, Parir2, Parir3);
	Result = FunctionCheckPerformance(DemA, DemB, DemC);
	printf("the result is %f \n", Result);
	printf("\\\\\\\\\\\\\\\end\\\\\\\\\\\\\\\\\ \n");

	while (1)
	{
		;
	}
}





//测试新权重 V2 ： 动态与静态模型权重 帮助原函数
void mainVK2(int NO, int Type)
{
	int count;

	char filenameWeight1[25] = "NWeight100.csv";

	char filenameWeight2[25] = "NWeight200.csv";

	char filenameWeight3[25] = "NWeight300.csv";

	char filenameWeight4[25] = "NBias100.csv";

	char filenameWeight5[25] = "NBias200.csv";

	char filenameWeight6[25] = "NBias300.csv";
	/*
	if (Type == 1)
	{
		filenameWeight1[0] = 'I';

		filenameWeight2[0] = 'I';

		filenameWeight3[0] = 'I';

		filenameWeight4[0] = 'I';

		filenameWeight5[0] = 'I';

		filenameWeight6[0] = 'I';


	}*/
	int Ten = int(NO / 10);
	int One = int(NO % 10);

	filenameWeight1[9] = One + '0';

	filenameWeight2[9] = One + '0';

	filenameWeight3[9] = One + '0';

	filenameWeight4[7] = One + '0';

	filenameWeight5[7] = One + '0';

	filenameWeight6[7] = One + '0';

	filenameWeight1[8] = Ten + '0';

	filenameWeight2[8] = Ten + '0';

	filenameWeight3[8] = Ten + '0';

	filenameWeight4[6] = Ten + '0';

	filenameWeight5[6] = Ten + '0';

	filenameWeight6[6] = Ten + '0';

	/*
	WriteToCSVB(NNode.ThiredB, 50, filenameWeight5);
	WriteToCSVB(NNode.Seond1B, 200, filenameWeight4);
	WriteToCSVB(NWeight.Fourth, 50 * 10, filenameWeight3);
	WriteToCSVB(NWeight.Thired, 200 * 50, filenameWeight2);
	WriteToCSVB(NWeight.Seond1, 200 * 784, filenameWeight1);
	WriteToCSVB(NNode.FourthB, 10, filenameWeight6);
*/
	

	
	//printf()
	count = ReadWeightAndBiasFromFiles(filenameWeight1, Seond1WND, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight2, ThiredWND, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight3, FourthWND, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight4, Seond1BND, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight5, ThiredBND, 784, 200);
	count = ReadWeightAndBiasFromFiles(filenameWeight6, FourthBND, 784, 200);

	printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n");

	double Result = 0;
	int DemA = 200;
	int DemB = 50;
	int DemC = 10;
	
	//ModifyWeightDyn(Parir1, Parir2, Parir3);
	//ModifyWeightFixB(Parir1, Parir2, Parir3);
	Result = FunctionCheckPerformance(DemA, DemB, DemC);
	printf("the result is %f \n", Result);
	printf("\\\\\\\\\\\\\\\end\\\\\\\\\\\\\\\\\ \n");

	while (1)
	{
		;
	}
}

/*  =============DAC 2021==============================*/

// defence
/* DAC_V1, fault injection with Flatten Defend*/

double Flaten_Def_FaultInjV1(int NO, int Type, int layA, int layB, int layC, int numberA, int numberB, int numberC)
{
	/*
		NO: 总测试次数
		Type: 0:随机添加， 1 最重要的添加
		layA,layB,layC 对三层网络节点添加的标志
		numberA,B,C 添加数目。
	*/
	printf("Test using Flaten_Def_FaultInjV1!\n");

	char WeightWA[25] = "001_WA_F.csv";
	char WeightWB[25] = "001_WB_F.csv";
	char WeightWC[25] = "001_WC_F.csv";
	char WeightBAC[25] = "001_BA_F.csv";
	char WeightBBC[25] = "001_BB_F.csv";
	char WeightBCC[25] = "001_BC_F.csv";
	int count = 0;
	count = ReadWeightAndBiasFromFiles(WeightWA, NWeight.Seond1, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightWB, NWeight.Thired, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightWC, NWeight.Fourth, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBAC, NNode.Seond1B, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBBC, NNode.ThiredB, 784, 200);
	count = ReadWeightAndBiasFromFiles(WeightBCC, NNode.FourthB, 784, 200);

	//select_node();

	/*============================Start====================================
	Protection:
	1.random
	2.adding noise
	==============================Start===============================*/
	printf("\n");
	printf("======%d====adding noise==%d=======\n", NO, numberA);
	printf("\n");
	int NumbOfNoise = 100;
	int AddingType = Type;   //0 randomly, 1->important first
	int Welen = 200 * 784;
	if (layA)
	{
		//AddingNoise(NWeight.Seond1, numberA, AddingType, Welen);
		//AddingNoise_def_flat_V1(double * buf, int number, int type, int TotLen, int * ProInd, int Len, int FlatFactor)
		//AddingNoise_def_flat_V1(NWeight.Seond1, numberA, AddingType, Welen, NodeIndA,NM,NP,200,80,0);
		AddingNoise_def_flat_V2(NWeight.Seond1, numberA, AddingType, Welen, NodeIndA, NM, NP, 200, 80, 0);
	}
	if (layB)
	{
		Welen = 200 * 50;
		//AddingNoise(NWeight.Thired, numberB, AddingType, Welen);
		//AddingNoise_def_flat_V1(NWeight.Thired, numberB, AddingType, Welen, NodeIndB, NM, NP, 50, 40, 0);
		AddingNoise_def_flat_V2(NWeight.Thired, numberB, AddingType, Welen, NodeIndB, NM, NP, 50, 40, 0);
	}
	if (layC)
	{
		Welen = 10 * 50;
		//AddingNoise_def_flat_V1(NWeight.Fourth, numberC, AddingType, Welen, NodeIndC, NM, NP, 10, 20, 0);
		AddingNoise_def_flat_V2(NWeight.Fourth, numberC, AddingType, Welen, NodeIndC, NM, NP, 10, 20, 0);
		//AddingNoise(NWeight.Fourth, numberC, AddingType, Welen);
	}
	//NWeight.Fourth;



	printf("==========adding noise  done===\n");
	//=========================准备训练图片和测试图片的label==================
	char filenameTrainLabel[25] = "TrainingLabel.txt";
	char filenameTeestLabeel[25] = "TestingLabel.txt";
	int TrainLabel[60000];
	int TestLabel[10000];
	TReadImageMNistLabel(filenameTrainLabel, TrainLabel);
	TReadImageMNistLabel(filenameTeestLabeel, TestLabel);

	//=================准备训练图片和测试图片=================
	double imageB[784] = { 0 };
	//ReadImage(filename);
	//image=ReadImageMNist(filename);
	char filenameO[25] = "TrainingImage0.txt";
	char filenameB[25] = "TrainingImage0.txt";
	char filenameOT[25] = "TestingImage0.txt";
	char filenameBT[25] = "TestingImage0.txt";
	int countImages = 60000;
	int testImages = 10000;
	int countImageIndex = 1;

	int Pre_Res;
	int T;
	int kID;
	int AccCount = 0;
	int i, j;
	for (countImageIndex = 0; countImageIndex < testImages; countImageIndex++)
	{
		CopyFilename(filenameBT, filenameOT);
		nameMakerT(filenameBT, countImageIndex);
		//printf("obtain the file of: %s     \n", filenameB);
		TReadImageMNistC(filenameBT, imageB);
		/*
		for (kID = 0; kID < 784; kID++)
		{
			//printf("%d     %f\n", kID, imageB[kID]);
			printf("%d     %f\n", kID, NWeight.Seond1[kID]);

		}
		*/
		//==========forward propagation================
		initNode();
		FClayerForward(imageB, NNode.Seond1, NWeight.Seond1, 784, 200, NNode.Seond1B);
		FClayerForwardRelu(NNode.Seond1, 200);
		FClayerForward(NNode.Seond1, NNode.Thired, NWeight.Thired, 200, 50, NNode.ThiredB);
		FClayerForwardRelu(NNode.Thired, 50);
		FClayerForward(NNode.Thired, NNode.Fourth, NWeight.Fourth, 50, 10, NNode.FourthB);
		//==========calculate the loss deviation================
		//ONode.SumofExp = ExpSumToProb(NNode.Fourth, ONode.Exp , 10, ONode.Prob);
		Pre_Res = findMax(NNode.Fourth, 10);
		T = TestLabel[countImageIndex];//获取groundtruth

		if (Pre_Res == T)
		{
			AccCount++;
			//printf("=====%d   ---------%d\n", Pre_Res, T);
		}


	}
	double Acc = double(double(AccCount) / double(testImages));
	printf("The recognition accuracy on CPU is %f\n", Acc);
	return Acc;

}


/*
mainVDAC_V1: 实现了基本功能，但是每次测试只针对每一层
*/

void mainVDAC_V1(bool DeOrAt)

{
	printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n");
	int TotSizeLayer[3] = { 784 * 200, 200 * 50, 50 * 10 };
	int PortionA[9] = { 1,2,3,4,5,6,7,8,9 };
	int PortionB[9] = { 1,2,3,4,5,6,7,8,9 };
	int PortionDef[9] = { 1,2,3,4,5,6,7,8,9 };
	int TestDefPort = 5;
	double Result[1000];
	double TMPres = 0;
	int Prolen = 2;
	int NO = 0;
	int layA;
	int layB;
	int layC;
	//三个层
/*
	//WriteTOCSV();
	double Test[10] = { 1,2,3,4,5,6,7,8,9,0 };
	AddingNoise(Test, 5, 1, 10);
	for (int i = 0; i < 10; i++)
	{
		printf("==%f==,", Test[i]);
	}

	while (1)
	{
		;
	}

	*/

	for (int i = 0; i < 3; i++)
	{
		if (i == 0)
		{
			layA = 1;
			layB = 0;
			layC = 0;
		}
		if (i == 1)
		{
			layA = 0;
			layB = 1;
			layC = 0;
			//Prolen = Prolen + 1;
		}
		if (i == 2)
		{
			layA = 0;
			layB = 0;
			layC = 1;
			Prolen = Prolen + 4;
		}
		//只攻击的
		if (DeOrAt == true)
		{
		/*
			for (int j = 0; j < 9; j++)
			{
				int number = int( TotSizeLayer[i] * PortionA[j] / 10);
				TMPres = FunctionAddNoise(NO, 0, layA, layB, layC, number, number, number);
				Result[NO] = TMPres;
				NO++;
			}
			*/
			for (int j = 0; j <9; j++)
			{
				int number = int(TotSizeLayer[i] * PortionB[j] / 100);
				TMPres = FunctionAddNoise(NO, 1, layA, layB, layC, number, number, number);
				Result[NO] = TMPres;
				NO++;
			}
		}
//只防御
		else
		{
			printf("testing Defence for DAC 、\n");
			for (int j = 0; j < 9; j++)
			{
				/*
				int number = int(TotSizeLayer[i] * PortionA[j] / 10);
				int numberB = int(TotSizeLayer[i] * PortionA[j] / 10 * TestDefPort / 10);
				printf(" >>>>>>>>>>>>>>>>>>>>>>>>checking numbers %d     ====    %d\n", number, numberB);
				//TMPres = FunctionAddNoiseAndDefenceV2(NO, 0, layA, layB, layC, number, number, number, numberB, Prolen);
				TMPres = Flaten_Def_FaultInjV1(NO, 0, layA, layB, layC, number, number, number);
				Result[NO] = TMPres;
				NO++;
				*/
			}
			for (int j = 0; j < 9; j++)
			{
				int number = int(TotSizeLayer[i] * PortionB[j] / 100);
				int numberB = int(TotSizeLayer[i] * PortionB[j] / 100 * TestDefPort / 10);
				//TMPres = FunctionAddNoiseAndDefenceV2(NO, 1, layA, layB, layC, number, number, number, numberB, Prolen);
				TMPres = Flaten_Def_FaultInjV1(NO, 1, layA, layB, layC, number, number, number);
				Result[NO] = TMPres;
				NO++;
			}
		}
		
		//break;
	}
	//break;
	printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n");
	if (DeOrAt == true)
	{
		char FileeSaveDAC[25] = "001_DAC_AttNewNP3.csv";
		WriteToCSVB(Result, NO, FileeSaveDAC);
	}
	else
	{
		char FileeSaveDAC[25] = "001_DAC_DefVBNew.csv";
		WriteToCSVB(Result, NO, FileeSaveDAC);
	}
	
	printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n"); printf("\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \n");

}


void main()
{
	//mainVK2(1, 0);
	//mainK11();
	//mainV8();
	//mainV12();
	//mainV11();
	mainVDAC_V1(true);
	printf("=====s====\n");
	//select_node();
	while (1)
	{
		;
	}
}