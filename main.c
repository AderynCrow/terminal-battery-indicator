#include<stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

void printFirst();
void printSegment(char color[]);
void printLast();
char* getColor(int subPercentage);
void returnCursor();
void verticalPadding(int height);
void printPercentage();
void print_centered(char *text);

int segmentNumber = 4;
char red[] = "\033[0;41m";
char yellow[] = "\033[0;43m";
char green[] = "\033[0;42m";
char white[] = "\033[0;00m";

int getPercentage(){

    FILE *file;
    char buffer[4];
    int capacity;

    // Open the file for reading
    file = fopen("/sys/class/power_supply/BAT1/capacity", "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Read the content from the file
    if (fgets(buffer, 3, file) == NULL) {
        perror("Error reading file");
        fclose(file);
        return EXIT_FAILURE;
    }

    // Close the file
    fclose(file);

    // Convert the content to an integer
    return atoi(buffer);

}

char charging(){

	FILE *file;
	char buffer[10];
	char charging = ' ';

	file = fopen("/sys/class/power_supply/BAT1/status","r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }
	
    // Read the content from the file
    if (fgets(buffer, 3, file) == NULL) {
        perror("Error reading file");
        fclose(file);
        return EXIT_FAILURE;
    }
    // Close the file
    fclose(file);

	if (strstr(buffer, "C") != NULL)
		charging = '+';

    return charging;

}



int main(int argc, char **argv) {


 int aflag = 0;
  int bflag = 0;
  char *cvalue = NULL;
  int index;
  int c;

  opterr = 0;


  while ((c = getopt (argc, argv, "hs:")) != -1)
    switch (c)
      {
      case 'h':
		  printf("Options: -s <segments>\n");
		  return 0;
      case 's':
        segmentNumber = atoi(optarg);
        break;
      case '?':
        if (optopt == 's')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;
      default:
        abort ();
      }




	int currentPercentage;
	printf("\e[1;1H\e[2J");
	int i = 100;

	while (1) {
		returnCursor();
		verticalPadding(7);
		currentPercentage = getPercentage();
		printFirst();
		for (int i = segmentNumber-1; i >= 0;  i--){
			printSegment(getColor(currentPercentage-((100/segmentNumber)*i)));
		}
		printLast();
		printPercentage();
		sleep(1);
	}

	return 0;
}

void returnCursor(){
	printf("\e[0;0H");
}

void printPercentage(){
	char percentageString[5];
	sprintf(percentageString, "%c%d%% ",charging() , getPercentage());
	print_centered(percentageString);
}

void verticalPadding(int height) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int terminal_height = w.ws_row;

    int padding = (terminal_height - height) / 2;

    for (int i = 0; i < padding; i++) {
        printf("\n");
    }
}

void print_centered(char *text) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int terminal_width = w.ws_col;

    int padding = (terminal_width - strlen(text)) / 2;

    for (int i = 0; i < padding; i++) {
        printf(" ");
    }

    printf("%s", text);
}

void print_centered_offset(char *text) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int terminal_width = w.ws_col;

    int padding = (terminal_width - strlen(text) + 15) / 2;

    for (int i = 0; i < padding; i++) {
        printf(" ");
    }

    printf("%s", text);
}

char* getColor(int subPercentage){
	if(subPercentage>((100/segmentNumber)/3)*2)
		return green;
	if(subPercentage>(100/segmentNumber)/3)
		return yellow;
	if (subPercentage>0)
		return red;
	return white;
}


void printFirst(){
	printf("\033[0;37m");
	printf(white);
	print_centered("+-+\n");
	print_centered("+-+ +-+\n");
}

void printSegment(char color[]){
	char s[20];
	strcpy(s, "|");
	strcat(s, color);
	strcat(s, "     ");
	strcat(s, white);
	strcat(s, "|\n\0");
	print_centered_offset(s);
}

void printLast(){
	print_centered("+-----+\n");
}
