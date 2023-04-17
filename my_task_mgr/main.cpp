#include "task_mgr.h"
#include "fun.h"

int main() {

	cout << "\n\tIf you need help managing the program, enter 'h'\n";
	cout << "\n\n\tEnter any button to continue\n\tGood luck!:D\n";
	getchar();

	print_process();

	while (1) {

		thread phone_proc(takt_process);
		phone_proc.detach();

		string choice;
		cin.clear(); cin >> choice;
		while (choice.size() > 1) {
			cout << "No choice" << endl;
			cin >> choice;
		}
		switch (choice[0]) {
		case 'p': {
			print_process();
			break;
		}
		case 't': {
			if (term(print_process()) == true) {
				Sleep(1000);
				print_process();
			}
			else print_process();
			break;
		}
		case 'l': {
			(load_proc(print_process()));
			break;
		}
		case 'h': {
			system("cls");
			cout << "\t\tShort guide!\n\n";
			cout << "Enter 't' to terminate process\nEnter 'l' to get the processor load per processor\n";
			cout << "Enter 'g' to get threads id\n";
			cout << "Enter 'p' to update the list yourself\n";
			cout << "Enter 'q' to exit\n";
			cout << "\n\n\tEnter any button to continue\n";
			getchar(); getchar();
			print_process();
			break;
		}
		case 'g': {
			get_id_threads(print_process());
			break;
		}
		case'q': {
			system("cls");
			cout << "\t\nEnd.\n";
			exit(0);
		}
		default:
			cout << "No choice" << endl;
		}
	}

	return 0;
}