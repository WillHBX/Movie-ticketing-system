#include <iostream>
#include <iomanip>
#include <fstream>
using namespace::std;

struct MemberRecord
{
   char email[ 40 ];
   char password[ 24 ];
   char IDNumber[ 12 ];
   char name[ 12 ];
   char phone[ 12 ];
};

struct Movie
{
   int movieCode;
   int prices[ 4 ]; // prices[0]:adult, prices[1]:concession, prices[2]:disability, prices[3]:elderly
   bool dates[ 9 ];
   bool sessionTimes[ 17 ];
   bool occupiedSeats[ 9 ][ 17 ][ 8 ][ 12 ]; // occupiedSeats[i][j] is the occupied Seats for all accounts
};                                           // at j-th session time on i-th date

struct BookingInfo
{
   char email[ 40 ];
   int movieCode;
   int dateCode;
   int sessionTimeCode;
   int numTickets[ 4 ]; // numTickets[0]: the number of adult tickets,
                        // numTickets[1]: the number of concession tickets,
                        // numTickets[2]: the number of disability tickets,
                        // numTickets[3]: the number of elderly tickets
   char seletedSeats[ 24 ][ 4 ]; // seleted seats for the user with the specified email
};

char hours[ 17 ][ 8 ] = { "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00", "17:00",
                          "18:00", "19:00", "20:00", "21:00", "22:00", "23:00", "00:00", "01:00" };

void loadMemberInfo( MemberRecord memberDetails[], int &numMembers );
int inputAnInteger( int begin, int end );
void signIn( MemberRecord memberDetails[], int numMembers, Movie movies[], char movieNames[][ 60 ], int numMovies,
             BookingInfo bookingHistories[], int &numBookings, char availableDates[][ 12 ], int numDates );
bool illegal( char email[], char password[], MemberRecord memberDetails[], int numMembers, int &recordNumber );
void accountInfor( MemberRecord memberDetails[], int numMembers, int recordNumber );
void newMember( MemberRecord memberDetails[], int &numMembers );
bool existingID( char newIDNumber[], MemberRecord memberDetails[], int &numMembers );
bool existingEmail( char newEmail[], MemberRecord memberDetails[], int &numMembers );

void loadBookingHistories( BookingInfo bookingHistories[], int &numBookings );
void loadAvailableDates( char availableDates[][ 12 ], int &numDates );
void loadMovies( Movie movies[], int &numMovies );
void loadMovieNames( char movieNames[][ 60 ], int numMovies );
void buyTickets( BookingInfo bookingHistories[], int &numBookings, Movie movies[], char movieNames[][ 60 ],
                 int numMovies, char availableDates[][ 12 ], int numDates, char email[] );
void selectSeats( BookingInfo bookingHistories[], int numBookings, Movie movies[] );
void display( Movie movies[], BookingInfo bookingHistory );
void displaySessionTimes( Movie movies[], char movieNames[][ 60 ], int numMovies,
                          char availableDates[][ 12 ], int numDates );
void displayBookingHistory( BookingInfo bookingHistories[], int numBookings, Movie movies[],
                            char movieNames[][ 60 ], char availableDates[][ 12 ], char email[] );
void saveMemberInfo( MemberRecord memberDetails[], int numMembers );
void saveMovies( Movie movies[], int numMovies );
void saveBookingHistories( BookingInfo bookingHistories[], int numBookings );

int main()
{
	MemberRecord memberDetails[100] = {};
	int numMembers = 0;
	loadMemberInfo(memberDetails, numMembers);

	BookingInfo bookingHistories[1000] = {};
	int numBookings = 0;
	loadBookingHistories(bookingHistories, numBookings);

	Movie movies[30] = {};
	int numMovies = 0;
	loadMovies(movies, numMovies);

	char movieNames[30][60] = {};
	loadMovieNames(movieNames, numMovies);

	char availableDates[10][12] = {};
	int numDates = 0;
	loadAvailableDates(availableDates, numDates);

	cout << "Welcome to Vieshow Cinemas Taipei QSquare system\n";

	int choice;

	while (true)
	{
		cout << "\nEnter your choice:" << endl;
		cout << "1. Sign In\n";
		cout << "2. New Member\n";
		cout << "3. End\n? ";

		choice = inputAnInteger(1, 3);
		cout << endl;

		switch (choice)
		{
		case 1:
			signIn(memberDetails, numMembers, movies, movieNames, numMovies, bookingHistories, numBookings,
				availableDates, numDates);
			break;

		case 2:
			newMember(memberDetails, numMembers);
			break;

		case 3:
			saveMemberInfo(memberDetails, numMembers);
			saveMovies(movies, numMovies);
			saveBookingHistories(bookingHistories, numBookings);
			cout << "Thank you...\n\n";
			system("pause");
			return 0;

		default:
			cout << "Input Error!\n\n";
			break;
		}
	}

	system("pause");
}
//Ū���|�����
void loadMemberInfo(MemberRecord memberDetails[], int &numMembers)
{
	ifstream inMemberInfor("Member Info.dat", ios::in | ios::binary);		//�}��

	if (!inMemberInfor) {
		cerr << "\"Member Info.dat\" could not be opened." << endl;
		exit(1);
	}

	while (inMemberInfor.read(reinterpret_cast<char *>(&memberDetails[numMembers]), sizeof(MemberRecord)))	//Ū��
		numMembers++;					//�p��|����

	inMemberInfor.close();
}
//��J�ﶵ
int inputAnInteger(int begin, int end)
{
	char input[10] = {};
	int num = -1;
	cin >> input;

	if (input[2] != '\0')
		return -1;
	if (input[1] != '\0')
		num = (input[0] - '0') * 10 + (input[1] - '0');
	else
		num = input[0] - '0';

	if (!(num >= begin && num <= end))			//�d��~�^��-1
		num = -1;

	return num;
}
//�n�J�ÿ�ܭn�F��
void signIn(MemberRecord memberDetails[], int numMembers, Movie movies[], char movieNames[][60], int numMovies,
			BookingInfo bookingHistories[], int &numBookings, char availableDates[][12], int numDates)
{
	MemberRecord input = { "","","","","" };		//���J���b���K�X
	int recordMember = 0;

	while (true) {
		cout << "Enter email address: ";
		cin >> input.email;
		cout << "Enter password: ";
		cin >> input.password;

		if (illegal(input.email, input.password, memberDetails, numMembers, recordMember))	//	�P�_�b���K�X���S����
			cout << "\nSorry, unrecognized email or password.\n" << endl;
		else
			while (true) {
				cout << "\nEnter your choice:" << endl;
				cout << "1. Account Information" << endl;
				cout << "2. Buy Tickets" << endl;
				cout << "3. My Bookings" << endl;
				cout << "4. Sign Out\n?";	

				int choice = inputAnInteger(1, 4);				//��J�ﶵ
				switch (choice)
				{
				case 1:
					accountInfor(memberDetails, numMembers, recordMember); //�b����T
					break;

				case 2:
					buyTickets(bookingHistories, numBookings, movies, movieNames, numMovies, availableDates, numDates, input.email);	//�R��
					cout << "Successful!\n\n";
					break;

				case 3:
					displayBookingHistory(bookingHistories, numBookings, movies, movieNames, availableDates, input.email);	//��ܭq�����
					break;

				case 4:
					return;

				default:
					cout << "Input Error!\n\n";
					break;
				}
			}
	}
}

//	�P�_��J���b���K�X���S�����~
bool illegal(char email[], char password[], MemberRecord memberDetails[], int numMembers, int &recordNumber)
{
	for (int i = 0; i < numMembers; i++)					//���C�@��email
		for (int j = 0; j < 40; j++) {
			if (email[j] != memberDetails[i].email[j])		//email���ŦX���� ���U�@�����
				break;
			if (memberDetails[i].email[j]=='\0'&&email[j]=='\0') {									//���Sbreak�N��email�۲�
				recordNumber = i + 1;							//recordNumber�N��ĴX�����
				for (int i = 0; i < numMembers; i++)		//���C�@��password
					for (int j = 0; i < 12; j++) {
						if (password[j] != memberDetails[i].password[j])	//���ŦX���� ���U�@�����
							break;
						if (password[j]=='\0'&&memberDetails[i].password[j]=='\0')						//�L�~�h�^��false
							return 0;
					}
			}
		}
	return 1;
}
//�L�X�|����� ��ܭק�
void accountInfor(MemberRecord memberDetails[], int numMembers, int recordNumber)
{
	char email[40] = {};
	char id[12] = {};

	while (true) {
		cout << "\n1.Name: " << memberDetails[recordNumber - 1].name << endl;
		cout << "2.Email Address: " << memberDetails[recordNumber - 1].email << endl;
		cout << "3.Phone Number: " << memberDetails[recordNumber - 1].phone << endl;
		cout << "4.ID Number:" << memberDetails[recordNumber - 1].IDNumber << endl;
		cout << "5.Password: " << memberDetails[recordNumber - 1].password << endl;
		cout << "\nWhich one do you want to modify( 0-not modify)?";

		int choice = inputAnInteger(0, 5);				//��J�ﶵ
		switch (choice)
		{
		case 0:
			return;

		case 1:
			cout << "Enter correct data: ";
			cin >> memberDetails[recordNumber - 1].name;	//�ק����ƪ�name
			cout << "\nSuccessful!\n";
			return;

		case 2:
			cout << "Enter correct data: ";
			cin >> email;	//�ק����ƪ�email
			while (existingEmail(email, memberDetails, numMembers)) {
				cout << "\nAn account already exists with the email address!\n" << endl;
				cout << "Enter correct data: ";
				cin >> email;
			}
			memcpy(memberDetails[recordNumber - 1].email, email, 40);		//�N���L����ƽƻs��memberDetails
			cout << "\nSuccessful!\n";
			return;

		case 3:
			cout << "Enter correct data: ";
			cin >> memberDetails[recordNumber - 1].phone;		//�ק����ƪ�phone
			cout << "\nSuccessful!\n";
			return;

		case 4:
			cout << "Enter correct data: ";
			cin >>id;	//�ק����ƪ�IDNumber
			while (existingID(id, memberDetails, numMembers)) {
				cout << "\nAn account already exists with the ID number!\n" << endl;
				cout << "Enter correct data: ";
				cin >> id;
			}
			memcpy(memberDetails[recordNumber - 1].IDNumber, id, 12);
			cout << "\nSuccessful!\n";
			return;

		case 5:
			cout << "Enter correct data: ";
			cin >> memberDetails[recordNumber - 1].password;	//�ק����ƪ�password
			cout << "\nSuccessful!\n";
			return;

		default:
			cout << "Input Error! Please try again: ";
			break;
		}
	}
}
//�[�J�s�|��
void newMember(MemberRecord memberDetails[], int &numMembers)
{
	MemberRecord input = { "","","","","" };		//�s���J�����

	while (true) {
		cout << "Enter your ID number: ";
		cin >> input.IDNumber;
		while (existingID(input.IDNumber, memberDetails, numMembers)) {		//�P�_�ӽЪ�ID���S������
			cout << "\nAn account already exists with the ID number!\n" << endl;
			cout << "Enter your ID number: ";
			cin >> input.IDNumber;
		}

		cout << "Enter your name: ";
		cin >> input.name;

		cout << "Enter an email address: ";
		cin >> input.email;
		while (existingEmail(input.email, memberDetails, numMembers)) {		//�P�_�ӽЪ�email���S������
			cout << "\nAn account already exists with the email address!\n" << endl;
			cout << "Enter an email address: ";
			cin >> input.email;
		}

		cout << "Enter a password: ";
		cin >> input.password;

		cout << "Enter your phone number: ";
		cin >> input.phone;

		numMembers++;								//�h�@�ӷ|��
		memberDetails[numMembers - 1] = input;		//���Ҧ���ƪ��̫᭱
		cout << "\nSuccessful!" << endl << endl;
		break;
	}
}
//�P�_�O�_�s�b�ۦPid
bool existingID(char newIDNumber[], MemberRecord memberDetails[], int &numMembers)
{
	for (int i = 0; i < numMembers; i++)		//���C�@�����
		for (int j = 0; j < 12; j++) {
			if (newIDNumber[j] != memberDetails[i].IDNumber[j])	//���ŴN���U�@��
				break;
			if (newIDNumber[j]=='\0'&&memberDetails[i].IDNumber[j]=='\0')
				return true;
		}
	return false;
}
//�P�_�O�_�s�b�ۦPemail
bool existingEmail(char newEmail[], MemberRecord memberDetails[], int &numMembers)
{
	for (int i = 0; i < numMembers; i++)		//���C�@�����
		for (int j = 0; j < 40; j++) {
			if (newEmail[j] != memberDetails[i].email[j])	//���ŴN���U�@��
				break;
			if (newEmail[j]=='\0'&&memberDetails[i].email[j]=='\0')
				return true;
		}
	return false;
}

//���J�q������
void loadBookingHistories(BookingInfo bookingHistories[], int &numBookings)
{
	ifstream inBookingHistories("Booking Histories.dat", ios::in | ios::binary);	

	if (!inBookingHistories) {
		cerr << "\"Booking Histories.dat\" could not be opened." << endl;
		exit(1);
	}

	while (inBookingHistories.read(reinterpret_cast<char *>(&bookingHistories[numBookings]), sizeof(MemberRecord)))
		numBookings++;

	inBookingHistories.close();
}
//���J���
void loadAvailableDates(char availableDates[][12], int &numDates)
{
	ifstream inAvailableDates("Available Dates.txt", ios::in);

	if (!inAvailableDates) {
		cerr << "\"Available Dates.txt\" could not be opened." << endl;
		exit(1);
	}

	while (inAvailableDates.getline(availableDates[numDates],12,'\n'))
		numDates++;

	inAvailableDates.close();
}
//���J�q�v���
void loadMovies(Movie movies[], int &numMovies)
{
	ifstream inMovies("Movies.dat", ios::in | ios::binary);

	if (!inMovies) {
		cerr << "\"Movies.dat\" could not be opened." << endl;
		exit(1);
	}

	while (inMovies.read(reinterpret_cast<char *>(&movies[numMovies]), sizeof(Movie)))
		numMovies++;

	inMovies.close();
}
//���J�q�v�W��
void loadMovieNames(char movieNames[][60], int numMovies)
{
	ifstream inMovieNames("Movie Names.txt", ios::in);

	if (!inMovieNames) {
		cerr << "\"Movie Names.txt\" could not be opened." << endl;
		exit(1);
	}
	for (int i = 0; i < numMovies; i++)
		inMovieNames.getline(movieNames[i], 60, '\n');
		

	inMovieNames.close();
}
//�R��
void buyTickets(BookingInfo bookingHistories[], int &numBookings, Movie movies[], char movieNames[][60],
				int numMovies, char availableDates[][12], int numDates, char email[])
{
	displaySessionTimes(movies, movieNames, numMovies, availableDates, numDates);		//�L�X�q�v�C��

	memcpy(bookingHistories[numBookings].email, email, 40);								//�N�R�����H�c�s���ʲ�������

	cout << "\nEnter movie code (0 - 11): ";
	bookingHistories[numBookings].movieCode = inputAnInteger(0, 11);					//��q�v
	while (bookingHistories[numBookings].movieCode == -1) {
		cout << "Enter movie code (0 - 11): ";
		bookingHistories[numBookings].movieCode = inputAnInteger(0, 11);
	}
	cout << endl;

	cout << "Enter date code (0 - 8): ";												//����
	bookingHistories[numBookings].dateCode = inputAnInteger(0, 8);
	while (bookingHistories[numBookings].dateCode == -1 
			|| !movies[bookingHistories[numBookings].movieCode].dates[bookingHistories[numBookings].dateCode]) {
		cout << "Enter date code (0 - 8): ";
		bookingHistories[numBookings].dateCode = inputAnInteger(0, 8);
	}
	cout << endl;

	cout << "Enter session time code (0 - 16): ";										//��ɶ�
	bookingHistories[numBookings].sessionTimeCode = inputAnInteger(0, 16);
	while (bookingHistories[numBookings].sessionTimeCode == -1 
			|| !movies[bookingHistories[numBookings].movieCode].sessionTimes[bookingHistories[numBookings].sessionTimeCode]) {
		cout << "Enter session time code (0 - 16): ";
		bookingHistories[numBookings].sessionTimeCode = inputAnInteger(0, 16);
	}
	cout << endl;

	cout << "Movie: ";														//�L�X��ܪ���T
	for (int i = 0; i < 60; i++)
		cout << movieNames[bookingHistories[numBookings].movieCode][i];
	cout << endl;

	cout << "Date: ";
	for (int i = 0; i < 12; i++)
		cout << availableDates[bookingHistories[numBookings].dateCode][i];
	cout << endl;

	cout << "Show Time: ";
	for (int i = 0; i < 8; i++)
		cout << hours[bookingHistories[numBookings].sessionTimeCode][i];
	cout << endl;

	cout << "Price: " << "Adult-" << movies[bookingHistories[numBookings].movieCode].prices[0]	//�L�X����
		<< ", " << "Concession-" << movies[bookingHistories[numBookings].movieCode].prices[1]
		<< ", " << "Disability-" << movies[bookingHistories[numBookings].movieCode].prices[2]
		<< ", " << "Elderly-" << movies[bookingHistories[numBookings].movieCode].prices[3] << endl;
	cout << endl;

	bool flag = 0;										//flag�P�_�ʶR���ƬO�_������0
	while (flag == 0) {
		flag = 1;
		cout << "Enter the number of adult tickets (0 - 6): ";					
		bookingHistories[numBookings].numTickets[0] = inputAnInteger(0, 6);
		while (bookingHistories[numBookings].numTickets[0] == -1) {
			cout << "Enter the number of adult tickets (0 - 6): ";
			bookingHistories[numBookings].numTickets[0] = inputAnInteger(0, 6);
		}

		cout << "Enter the number of concession tickets (0 - 6): ";
		bookingHistories[numBookings].numTickets[1] = inputAnInteger(0, 6);
		while (bookingHistories[numBookings].numTickets[1] == -1) {
			cout << "Enter the number of concession tickets (0 - 6): ";
			bookingHistories[numBookings].numTickets[1] = inputAnInteger(0, 6);
		}

		cout << "Enter the number of disability tickets (0 - 6): ";
		bookingHistories[numBookings].numTickets[2] = inputAnInteger(0, 6);
		while (bookingHistories[numBookings].numTickets[2] == -1) {
			cout << "Enter the number of disability tickets (0 - 6): ";
			bookingHistories[numBookings].numTickets[2] = inputAnInteger(0, 6);
		}

		cout << "Enter the number of elderly tickets (0 - 6): ";
		bookingHistories[numBookings].numTickets[3] = inputAnInteger(0, 6);
		while (bookingHistories[numBookings].numTickets[3] == -1) {
			cout << "Enter the number of elderly tickets (0 - 6): ";
			bookingHistories[numBookings].numTickets[3] = inputAnInteger(0, 6);
		}
		cout << endl;
		
		for (int i = 0; i < 4; i++) {
			if (bookingHistories[numBookings].numTickets[i] != 0)
				break;
			flag = 0;
		}
	}
	display(movies,bookingHistories[numBookings]);		//�L�X�ʲ���T
	selectSeats(bookingHistories, numBookings, movies);	//��y��
	numBookings++;										//�h�@���q�����
}

//��y��
void selectSeats(BookingInfo bookingHistories[], int numBookings, Movie movies[])
{
	char ch = 'A';
	cout << " ";
	for (int i = 0; i < 12; i++,ch++)			//�L�XA~L�C
		cout  << setw(2) << ch ;
	cout << endl;
	for (int i = 0; i < 8; i++) {				
		cout << i;								//�L�X��
		for (int j = 0; j < 12; j++)
			cout << setw(2)						//�L�X�C�Ӯy�쪺bool��
				 << movies[bookingHistories[numBookings].movieCode].occupiedSeats[bookingHistories[numBookings].dateCode][bookingHistories[numBookings].sessionTimeCode][i][j];
		cout << endl;
	}
	int numTickets = 0;		
	for (int i = 0; i < 4; i++)
		numTickets += bookingHistories[numBookings].numTickets[i];	//�p���`����
	cout << "\nSelect " << numTickets << " seats (e.g. 0A):\n";
	for (int i = 0; i < numTickets; i++) {							//��J�y��
		cout << "?";
		char input[24][4] = {""};
		cin >> input[i];
		while (input[i][2] != '\0'|| input[i][0]>'7'|| input[i][0]<'0'|| input[i][1]>'L'|| input[i][1]<'A') {
			cout << "Input Error! Enter again.\n";
			cin >> input[i];
		}
		//�y��w�g���H�R���ɭ԰���j��
		while (movies[bookingHistories[numBookings].movieCode].occupiedSeats[bookingHistories[numBookings].dateCode][bookingHistories[numBookings].sessionTimeCode][input[i][0] - '0'][input[i][1] - 'A']) {
			cout << "This seat bas been occupied. Please select another seat.\n?";
			cin >> input[i];
			while (input[i][2] != '\0' || input[i][0]>'7' || input[i][0]<'0' || input[i][1]>'L' || input[i][1]<'A') {
				cout << "Input Error! Enter again.\n";
				cin >> input[i];
			}
		}
		memcpy(bookingHistories[numBookings].seletedSeats[i], input[i], 2);		//��J���y��S���D���ܡA�s�J�ʲ�����
		//�N�ʶR���y��]��1
		movies[bookingHistories[numBookings].movieCode].occupiedSeats[bookingHistories[numBookings].dateCode][bookingHistories[numBookings].sessionTimeCode][input[i][0] - '0'][input[i][1] - 'A'] = 1;
	}
}
//�L�X�ʲ���T
void display(Movie movies[], BookingInfo bookingHistory)
{
	int sum = 0;
	cout  << setw(37) << "No. of Tickets" << setw(7) << "Price" << setw(10) << "Subtotal" << endl;
	for (int i = 0; i < 4; i++) {
		if (bookingHistory.numTickets[i] != 0) {		//�ʶR�i�ƬO0�����L
			cout << left << setw(27);
			switch (i)
			{
			case 0:
				cout << "Adult";
				break;
			case 1:
				cout << "Concession tickets";
				break;
			case 2:
				cout << "Disability tickets";
				break;
			case 3:
				cout << "Elderly tickets";
				break;
			default:
				break;
			}
			cout << right << setw(10) << bookingHistory.numTickets[i] << setw(7) << movies[bookingHistory.movieCode].prices[i]
				<< setw(10) << bookingHistory.numTickets[i] * movies[bookingHistory.movieCode].prices[i] << endl;
		}
			sum += bookingHistory.numTickets[i] * movies[bookingHistory.movieCode].prices[i];			//�p���`����
		}
	cout << "\nTotal Amount For Tickets: " << sum << endl << endl;
}

//�L�X�q�v��T
void displaySessionTimes(Movie movies[], char movieNames[][60], int numMovies,
						 char availableDates[][12], int numDates)
{
	for (int i = 0; i < numMovies; i++) {
		cout << endl;
		cout << setw(2) << movies[i].movieCode << ". Movie:  " << movieNames[i];
		cout << endl << "    Date:";
		for (int j = 0; j < numDates; j++)
			if (movies[i].dates[j]) {
				cout << setw(2) << j << ". " << availableDates[j] << ",";
			}
		cout << endl << "    Sesssion Time:";
		for (int j = 0; j < 17; j++) 
			if (movies[i].sessionTimes[j]) {
				cout << setw(2) << j << ". "<< hours[j] << ",";
			}
		cout << endl;
	}
}
//�L�X�ʲ�����
void displayBookingHistory(BookingInfo bookingHistories[], int numBookings, Movie movies[],
	char movieNames[][60], char availableDates[][12], char email[])
{
	int flag = 0;
	for (int i = 0; i < numBookings; i++) {
		if (memcmp(bookingHistories[i].email, email, 40) == 0) {		//�P�_email�O�_�۲�
			if(flag == 0)
				cout << "\nBooking History:\n";
			flag = 1;
			cout << "Movie: " << movieNames[bookingHistories[i].movieCode] << endl;
			cout << "Date: " << availableDates[bookingHistories[i].dateCode] << endl;
			cout << "Show Time: " << hours[bookingHistories[i].sessionTimeCode] << endl;
			cout << "Seats: ";
			for (int j = 0; j < 24; j++)
				if (movies[bookingHistories[i].movieCode].occupiedSeats[bookingHistories[i].dateCode][bookingHistories[i].sessionTimeCode][bookingHistories[i].seletedSeats[j][0] - '0'][bookingHistories[i].seletedSeats[j][1] - 'A'] == 1)
					cout << setw(3) << bookingHistories[i].seletedSeats[j] ;
			cout << endl <<endl;
			display(movies, bookingHistories[i]);
		}
	}
	if (flag == 0)
		cout << "\nNo Bookings!\n";
}
//�g�J�|�����
void saveMemberInfo(MemberRecord memberDetails[], int numMembers)
{
	ofstream outMemberInfor("Member Info.dat", ios::out | ios::binary);

	if (!outMemberInfor) {
		cerr << "\"Member Info.dat\" could not be opened." << endl;
		exit(1);
	}

	for (int i = 0; i < numMembers; i++) {
		outMemberInfor.write(reinterpret_cast<char *>(&memberDetails[i]), sizeof(MemberRecord));
	}
	outMemberInfor.close();
}
//�g�J�q�v���
void saveMovies(Movie movies[], int numMovies)
{
	ofstream outMovies("Movies.dat", ios::out | ios::binary);

	if (!outMovies) {
		cerr << "\"Movies.dat\" could not be opened." << endl;
		exit(1);
	}

	for (int i = 0; i < numMovies; i++)
		outMovies.write(reinterpret_cast<char *>(&movies[i]), sizeof(Movie));

	outMovies.close();
}
//�g�J�q������
void saveBookingHistories(BookingInfo bookingHistories[], int numBookings)
{
	ofstream outBookingHistories("Booking Histories.dat", ios::out | ios::binary);

	if (!outBookingHistories) {
		cerr << "\"Booking Histories.dat\" could not be opened." << endl;
		exit(1);
	}

	for (int i = 0; i < numBookings; i++) {
		outBookingHistories.write(reinterpret_cast<char *>(&bookingHistories[i]), sizeof(MemberRecord));
	}

	outBookingHistories.close();
}
