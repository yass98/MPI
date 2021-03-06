#include <mpi.h>
#include <iostream>
#include <math.h>
#include "Arifm.cpp"

using std::printf;

void lab_1(int argc, char* argv[]) {
	/*
	Реализуйте функцию star, которая создаёт N+1 процессов (1 «центральный» и N «крайних») и посылает сообщение центральному процессу,
	который посылает сообщение всем остальным процессам и дожидается от них ответа, после чего это повторяется (всего M раз).
	После того, как все события получены, все процессы заканчивают работу.*/

	int ProcNum, ProcRank, RecvRank;
	MPI_Status Status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum); // Число процессов
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank); // Номер процесса

	int M = 1; // Число повторов
	for (int k = 0; k < M; k++) {

		if (ProcRank == 0)
		{
			// Действия, выполняемые только процессом с рангом 0
			printf("Round %d. Hello text from process %d\n", k, ProcRank);

			for (int i = 1; i < ProcNum; i++)
			{
				printf("Round %d. Process %d sending to %d process message.. \n", k, ProcRank, i);
				MPI_Send(&ProcRank, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

				int Ans;
				MPI_Recv(&Ans, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
				printf("Round %d. Process %d received answer. Answer is %d \n", k, ProcRank, Ans);
			}
		}
		else
		{
			int Data;
			MPI_Recv(&Data, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
			printf("Round %d. Process %d received data. Data is %d \n", k, ProcRank, Data);

			int Recipient = 0;
			printf("Round %d. Process %d sending answer to %d process... \n", k, ProcRank, Recipient);
			MPI_Send(&ProcRank, 1, MPI_INT, Recipient, 0, MPI_COMM_WORLD);
		}

	}

	MPI_Finalize();
}

void lab_2_broadcast(int argc, char* argv[]) {
	/* Модифицировать программу, написанную на Л.Р. №1, так чтобы она работала на основе коллективной передачи сообщений */

	int ProcNum, ProcRank, RecvRank;
	MPI_Status Status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum); // Число процессов
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank); // Номер процесса

	int M = 1; // Число повторов
	for (int k = 0; k < M; k++) {
		int Data;
		if (ProcRank == 0) {
			Data = 0;
			printf("Round %d. Hello text from process %d\n", k, ProcRank);
			printf("Round %d. Process %d broadcasting to other processes message.. \n", k, ProcRank);
		}
		MPI_Bcast(&Data, 1, MPI_INT, 0, MPI_COMM_WORLD);

		if (ProcRank != 0) {
			printf("Round %d. Process %d received data. Data is %d \n", k, ProcRank, Data);

			int Recipient = 0;
			printf("Round %d. Process %d sending answer to %d process... \n", k, ProcRank, Recipient);
			MPI_Send(&ProcRank, 1, MPI_INT, Recipient, 0, MPI_COMM_WORLD);
		}
		else {
			for (int i = 1; i < ProcNum; i++)
			{
				int Ans;
				MPI_Recv(&Ans, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
				printf("Round %d. Process %d received answer. Answer is %d \n", k, ProcRank, Ans);
			}
		}
	}
	MPI_Finalize();
}

void lab_2_reduce(int argc, char* argv[]) {
	/* Модифицировать программу, написанную на Л.Р. №1, так чтобы она работала на основе коллективной передачи сообщений */

	int ProcNum, ProcRank, RecvRank;
	MPI_Status Status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum); // Число процессов
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank); // Номер процесса

	int M = 1; // Число повторов
	for (int k = 0; k < M; k++) {
		int Data;
		if (ProcRank == 0) {
			Data = 1;
			printf("Round %d. Hello text from root-process-%d\n", k, ProcRank);
			printf("Round %d. Root-process-%d broadcasting message (it's %d) to other processes... \n", k, ProcRank, Data);
		}
		MPI_Bcast(&Data, 1, MPI_INT, 0, MPI_COMM_WORLD);

		printf("Round %d. Process %d received data. Data is %d \n", k, ProcRank, Data);

		int Ans;
		MPI_Reduce(&Data, &Ans, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

		if (ProcRank == 0) {
			printf("Round %d. Root-process-%d received and collect answer. Answer is %d \n", k, ProcRank, Ans);
		}
	}
	MPI_Finalize();
}

void lab_3(int argc, char* argv[]) {

	int ProcNum, ProcRank, RecvRank;
	MPI_Status Status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum); // Число процессов
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank); // Номер процесса

	int k = 0;

	int* Data = new int[ProcNum];
	if (ProcRank == 0) {
		printf("Round %d. HELLO from process %d\n", k, ProcRank);
		printf("Round %d. Root-process-%d initialize data. Data is ", k, ProcRank);
		for (int i = 1; i < ProcNum; i++) {
			Data[i] = rand() % 100;
			printf("%d, ", Data[i]);
		}
		printf("\n");
		printf("Round %d. Root-process-%d sending data...\n", k, ProcRank);
	}

	int RcvData;
	MPI_Scatter(Data, 1, MPI_INT, &RcvData, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (ProcRank != 0) {
		printf("Round %d. Process %d received data. Data is %d \n", k, ProcRank, RcvData);
	}

	int AnsOne;
	int* AnsData = nullptr;
	if (ProcRank == 0) {
		AnsData = new int[ProcNum];
		printf("Round %d. Root-process-%d prepared to get all data data. \n", k, ProcRank);
	}
	else {
		AnsOne = RcvData % 10;
		printf("Round %d. Process %d calculated somithing. AnsData is %d \n", k, ProcRank, AnsOne);
	}
	MPI_Gather(&AnsOne, 1, MPI_INT, AnsData, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if (ProcRank == 0) {
		printf("Round %d. Root-process-%d received answer. Answer is ", k, ProcRank);
		for (int i = 1; i < ProcNum; i++) {
			printf("%d, ", AnsData[i]);
		}
		printf("\n");
	}

	MPI_Finalize();
}

void lab_4(int argc, char* argv[]) {
	/* Реализуйте тип «полином».
	Например 4x^3+2x^2-1x+7 - это полином 3-й степени. Следовательно нужно хранить степень и массив коэффициентов.
	Напишите программу, которая осуществляет умножение N полиномов заданной степени.
	*/

	int ProcNum, ProcRank, RecvRank;
	MPI_Status Status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum); // Число процессов
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank); // Номер процесса

	const int N = 3; //Число полиномов
	const int MAX_POL = 2; // Максимальная степень полинома

	int coefCount = MAX_POL + 1; // Количество коэффициентов, которые надо хранить
	MPI_Datatype newType;  // Новый тип для передачи объекта целиком
	MPI_Type_contiguous(coefCount, MPI_INT, &newType);
	MPI_Type_commit(&newType); // Объявляем тип

	if (ProcRank == 0) {
		printf("HELLO from process %d\n", ProcRank);
		printf("Root-process-%d initialize data. \n", ProcRank);
		printf("Data is: \n");
		int* arr = new int[N*coefCount];
		for (int i = 0; i < N*coefCount; i++) {
			arr[i] = i + 1;
			printf("%d", arr[i]);
			if (i % coefCount == coefCount - 1)
				printf("\n");
			else
				printf(", ");
		}

		int buflen;
		MPI_Pack_size(N*coefCount, MPI_INT, MPI_COMM_WORLD, &buflen);
		char* buf = new char[buflen];
		printf("Root-process-%d packing data in buffer of size %d bytes... \n", ProcRank, buflen);

		int bufpos = 0;
		for (int i = 0; i < N; i++) {
			//Упаковка:
			/*
			data – буфер памяти с элементами для упаковки,
			count – количество элементов в буфере,
			type – тип данных для упаковываемых элементов,
			buf - буфер памяти для упаковки,
			buflen – размер буфера в байтах,
			bufpos – позиция для начала записи в буфер (в байтах от начала буфера),
			comm - коммуникатор для упакованного сообщения.
			*/
			MPI_Pack(&(arr[i*coefCount]), 1, newType, buf, buflen, &bufpos, MPI_COMM_WORLD);
		}
		printf("Root-process-%d packed data into buffer. \n", ProcRank);


		for (int receiver = 1; receiver < ProcNum; receiver++)
		{
			printf("Root-process-%d sending data to process-%d... \n", ProcRank, receiver);
			MPI_Send(buf, bufpos, MPI_PACKED, receiver, 0, MPI_COMM_WORLD);
		}
		printf("\n");
		fflush(stdout);


		printf("Root-process-%d waiting answers... \n", ProcRank);
		int* ans = new int[coefCount];
		for (int proc = 1; proc < ProcNum; proc++)
		{
			int i = proc - 1;
			MPI_Recv(&(ans[i]), 1, MPI_INT, proc, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
			printf("Root-process-%d received answer %d from process %d. \n", ProcRank, ans[i], proc);
		}
		printf("Root-process-%d print answers: \n", ProcRank);
		for (int i = 0; i < coefCount; i++) {
			printf("%d, ", ans[i]);
		}

		printf("\n");
		delete[] arr;
		delete[] buf;
		MPI_Type_free(&newType); // Освобождаем тип 
	}
	else {
		int buflen = N * coefCount * sizeof(int);
		char* buf = new char[buflen];
		printf("Process %d waiting data of size %d bytes...\n", ProcRank, buflen);
		MPI_Recv(buf, buflen, MPI_PACKED, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
		printf("Process %d received data. Unpacking it...\n", ProcRank);

		int dataCount = N * coefCount;
		int* data = new int[dataCount];
		int pos = 0;
		for (int i = 0; i < dataCount; i++)
			MPI_Unpack(buf, buflen, &pos, &(data[i]), 1, MPI_INT, MPI_COMM_WORLD);

		printf("Data is: \n");
		for (int i = 0; i < dataCount; i++) {
			printf("%d", data[i]);
			if (i % coefCount == coefCount - 1)
				printf("\n");
			else
				printf(", ");
		}

		int ans = 0;
		int k = ProcRank - 1;
		for (int i = 0; i < N; i++) {
			ans += data[i*coefCount + k];
		}
		int receiver = 0;
		printf("Process %d sending answer to %d. \n", ProcRank, receiver);
		printf("Answer is %d...\n", ans);
		MPI_Send(&ans, 1, MPI_INT, receiver, 0, MPI_COMM_WORLD);

		printf("\n");
		delete[] buf;
		delete[] data;
	}


	MPI_Finalize();
}

void lab_5(int argc, char* argv[]) {
	/* Вариант 2.
	Реализуйте тип «длинное целое».
	Напишите программу, которая осуществляет умножение A целых чисел заданной длины методом Тоома-Кука.
	P.S. Можно реализовать простое умножение длинных чисел "столбиком"
	*/
	/* Затем, например, 8 чисел можно перемножить 4 процессами следующим образом:
	1   2  3   4	   5   6  7   8
	\ /    \ / 	    \ /    \ /
	[*]    [*] 	    [*]    [*]
	\     |         \     |
	---[*]          ---[*]
	\              |
	------------[*]
	|
	ответ
	*/

	int ProcNum, ProcRank, RecvRank;
	MPI_Status Status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum); // Число процессов
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank); // Номер процесса

	Arifm a;
	if (ProcRank == 0)
		for (int i = 0; i < ProcNum; i++) {
			lnum x = a.random(2);
			a.print(x);
		}

	MPI_Group world_group;
	MPI_Comm_group(MPI_COMM_WORLD, &world_group);

	int* ranks = new int(2);
	ranks[0] = 0;
	ranks[1] = 1;

	MPI_Group sub_group;
	MPI_Group_incl(world_group, 2, ranks, &sub_group);
	MPI_Comm sub_comm;
	MPI_Comm_create(MPI_COMM_WORLD, sub_group, &sub_comm);

	// ---ТЕОРИЯ---
	/*
	// --Группы--
	// Для получения группы, связанной с существующим коммуникатором, используется функция:
	//int MPI_Comm_group(MPI_Comm comm, MPI_Group *group)

	// создание новой группы newgroup из существующей группы oldgroup, которая будет включать в себя n процессов, ранги которых перечисляются в массиве ranks:
	//int MPI_Group_incl(MPI_Group oldgroup, int n, int *ranks, MPI_Group *newgroup) - включение
	//int MPI_Group_excl(MPI_Group oldgroup,int n, int *ranks, MPI_Group *newgroup) - исключение

	//int MPI_Group_union(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup) - объединение
	//int MPI_Group_intersection(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup) - пересечение
	//int MPI_Group_difference (MPI_Group group1, MPI_Group group2, MPI_Group *newgroup) - разница

	//MPI_COMM_EMPTY - пустая группа

	//int MPI_Group_size ( MPI_Group group, int *size ) - получение количества процессов в группе
	//int MPI_Group_rank(MPI_Group group, int *rank) - получение ранга текущего процесса в группе

	//int MPI_Group_free ( MPI_Group *group) - удаление группы и освобождение ресурсов

	// --Коммуникаторы--
	//int MPI_Comm_dup (MPI_Comm oldcom, MPI_comm *newcomm) - дублирование уже существующего коммуникатора
	//int MPI_Comm_create (MPI_Comm oldcom, MPI_Group group, MPI_Comm *newcomm) - создание нового коммуникатора из подмножества процессов существующего коммуникатора

	// Быстрый способ одновременного создания нескольких коммуникаторов:
	//  oldcomm – исходный коммуникатор,
	//  split – номер коммуникатора, которому должен принадлежать процесс,
	//  key – порядок ранга процесса в создаваемом коммуникаторе,
	//  newcomm – создаваемый коммуникатор.
	//int MPI_Comm_split ( MPI_Comm oldcomm, int split, int key, MPI_Comm *newcomm)

	//int MPI_Comm_free (MPI_Comm *comm) - удаление
	*/



	MPI_Finalize();
}

int main(int argc, char* argv[])
{
	//lab_1(argc, argv);
	//lab_2_broadcast(argc, argv);
	//lab_2_reduce(argc, argv);
	//lab_3(argc, argv);
	//lab_4(argc, argv);
	lab_5(argc, argv);

	return 0;
}


