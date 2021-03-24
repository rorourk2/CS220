#include "parser.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LINE_LENGTH (128+MAX_NAME_LENGTH)
#define MAX_NAME_LENGTH 32

struct student* s;

/*
 * This function reads the input file in text format and stores it in memory. 
 * See sample input file for format. 
 * Lines starting with '#' are comments and must be ignored. 
 * If there is an error in input text format, the error string must contain the
 * line number where the error occurred. 
 * Also, upon error, all allocated memory must be freed. 
 */
struct student *read_txt (char *filename, char *error_string){
	FILE *fp;
	fp = fopen(filename, "r");
	if(fp == NULL) {
		printf("Invalid Input\n");
		return 0;
	}
	
	unsigned int line_length = 0;
	unsigned int line_number = 0;
	struct student *head_student = (struct student *) malloc(0);
	s = NULL;
	
	char student_name[MAX_NAME_LENGTH];
	for(int i=0; i<MAX_NAME_LENGTH; i++){
		student_name[i]='\0';
	}
	unsigned int student_age;
	enum _SEX student_sex;
	float student_gpa;
	unsigned int course_number;
	unsigned char course_grade;
	
	/*these variables are used to determine if, at the current point in the file, the code is working within a student/course, and which student/course it is or would be working in should it be active*/
	unsigned int active_student = 0;
	unsigned int active_course = 0;
	short int issue = 0;
	
	char current_line[MAX_LINE_LENGTH+1];
	for(int i=0; i<MAX_LINE_LENGTH; i++){
		current_line[i]='\0';
	}
	fgets(current_line, MAX_LINE_LENGTH, fp);
	line_length = (unsigned int) strlen(current_line);
	line_number++;
	while(line_length > 0 && issue==0){
		char blank_word_1[MAX_NAME_LENGTH];
		char blank_word_2[MAX_NAME_LENGTH];
		char blank_word_3[MAX_NAME_LENGTH];
		char *words[3] = {blank_word_1, blank_word_2, blank_word_3};
		words[0][0]='\0';
		words[1][0]='\0';
		words[2][0]='\0';
		char current_word[MAX_NAME_LENGTH];
		for(int i=0; i<MAX_NAME_LENGTH; i++){
			current_word[i]='\0';
		}
		unsigned int word_index = 0;
		unsigned int char_index = 0;		
		for(int char_num=0; char_num<line_length; char_num++){
			char current_char = current_line[char_num];
			
			if(char_num==0 && current_char=='#'){
				for(int i=0; i<MAX_LINE_LENGTH; i++){
					current_line[i]='\0';
				}
				break;
			}
			if(current_char == ' ' || current_char == '\t' || current_char == '\n'){ /*If there's whitespace...*/
				if(char_index!=0){ /*... and an unsaved word...*/
					if(word_index>2){ /*... but not enough space to save the word,*/
						issue++;
						char *too_many_words = "Error: Too many words: Line ";
						for(int i=0; i<strlen(too_many_words); i++){
							error_string[i]=too_many_words[i];
						}
						unsigned int zeros = 0;
						unsigned int temp_num = line_number;
						while(temp_num!=0){
							zeros++;
							temp_num/=10;
						}
						for(int i=0; i<zeros; i++){
							temp_num=line_number;
							for(int j=0; j<i; j++){
								temp_num/=10;
							}
							temp_num%=10;
							error_string[i+strlen(too_many_words)]= temp_num + '0';
						}
						error_string[strlen(too_many_words)+zeros]='\n';
						free(too_many_words);
						break;
					} 
					else { /*... and there is enough space to save the word,*/
						strcpy(words[word_index], current_word);
						word_index++;
						for(int i=0; i<MAX_NAME_LENGTH; i++){
							current_word[i]='\0';
						}
						char_index = 0;
					}
				}
			}
			else{ /*If there isn't whitespace...*/
				current_word[char_index]=current_char;
				char_index++;
			}
			if(char_num==line_length-1){ /*If this is the last character of the line...*/
				if(char_index!=0){ /*... and there's an unsaved word...*/
					if(word_index>2){ /*... but there's already too many words,*/
						issue++;
						char *too_many_words = "Error: Too many words: Line ";
						for(int i=0; i<strlen(too_many_words); i++){
							error_string[i]=too_many_words[i];
						}
						unsigned int zeros = 0;
						unsigned int temp_num = line_number;
						while(temp_num!=0){
							zeros++;
							temp_num/=10;
						}
						for(int i=0; i<zeros; i++){
							temp_num=line_number;
							for(int j=0; j<i; j++){
								temp_num/=10;
							}
							temp_num%=10;
							error_string[i+strlen(too_many_words)]= temp_num + '0';
						}
						error_string[strlen(too_many_words)+zeros]='\n';
						free(too_many_words);
						break;
					} 
					else { /*... and there's room to save a new word,*/
						strcpy(words[word_index], current_word);
						for(int i=0; i<sizeof(*current_word); i++){
							current_word[i]='\0';
						}
						word_index++;
						char_index = 0;
					}
				}
			}
		}/*End of For Loop (line loop)*/
		if(!strcmp(words[0], "}")){
			if(active_course && active_student){
				active_course=0;
				add_course(student_name, course_number, course_grade);
			} else if (active_student){
				active_student=0;
			} else {
				goto Syntax_Error;
			}
		} else if(!strcmp(words[0], "student")){
		if(!strcmp(words[1], "{")){
				/*change the working student structure to fit with the given data*/
				if(!active_student && !active_course){
					active_student = 1;
					student_age = 0;
					student_gpa = 0.0;
					for(int i=0; i<MAX_NAME_LENGTH; i++){
						student_name[i]='\0';
					}
				} else {
					goto Syntax_Error;
				}
			} else {
				goto Word_Error;
			}
		} else if(!strcmp(words[0], "course")){
			if(!strcmp(words[1], "{")){
				/*change the working course structure within the working student structure to fit with the given data*/
				if(!active_course){
					active_course = 1;
					course_number = 0;
					course_grade = 'N';
					if(active_student==1){
						active_student=2;
						enroll_student(student_name, student_age, student_sex, student_gpa);
					}
				} else {
					goto Syntax_Error;
				}
			} else {
				goto Word_Error;
			}
		} else if(!strcmp(words[0], "name")){
			if(!strcmp(words[1], "=")){
				/*set the name field within the working student*/
				if(active_student){
					for(int i=0; i<strlen(words[2]); i++){
						student_name[i]=words[2][i];
					}
				} else {
					goto Syntax_Error;
				}
			} else {
				goto Word_Error;
			}
		} else if(!strcmp(words[0], "age")){
			if(!strcmp(words[1], "=")){
				/*set the age field within the working student*/
				if(active_student){
					student_age=atoi(words[2]);
				} else {
					goto Syntax_Error;
				}
			} else {
				goto Word_Error;
			}
		} else if(!strcmp(words[0], "sex")){
			if(!strcmp(words[1], "=")){
				/*set the sex field within the working student*/
				if(active_student){
					if(!strcmp(words[2], "M")){
						student_sex = 'M';
					} else if(!strcmp(words[2], "F")){
						student_sex = 'F';
					} else if(!strcmp(words[2], "O")){
						student_sex = 'O';
					} else {
						goto Word_Error;
					}
				} else {
					goto Syntax_Error;
				}
			} else {
				goto Word_Error;
			}
		} else if(!strcmp(words[0], "gpa")){
			if(!strcmp(words[1], "=")){
				/*set the gpa field within the working student*/
				if(active_student){
					student_gpa = (float) atof(words[2]);
				} else {
					goto Syntax_Error;
				}
			} else {
				goto Word_Error;
			}
		} else if(!strcmp(words[0], "number")){
			if(!strcmp(words[1], "=")){
				/*set the number field within the working course within the working student*/
				if(active_student && active_course){
					course_number = atoi(words[2]);
				} else {
					goto Syntax_Error;
				}
			} else {
				goto Word_Error;
			}
		} else if(!strcmp(words[0], "grade")){
			if(!strcmp(words[1], "=")){
				/*set the grade field within the working course within the working student*/
				if(active_student && active_course){
					course_grade = words[2][0];
				} else {
					goto Syntax_Error;
				}
			} else {
				goto Word_Error;
			}
		}
		for(int i=0; i<MAX_LINE_LENGTH; i++){
			current_line[i]='\0';
		}
		fgets(current_line, MAX_LINE_LENGTH, fp);
		line_length = (unsigned int) strlen(current_line);
		line_number++;
		continue;
		
		Syntax_Error: ;
		issue++;
		char *invalid_syntax = "Error: Invalid Syntax:  Line ";
		for(int i=0; i<strlen(invalid_syntax); i++){
			error_string[i]=invalid_syntax[i];
		}
		unsigned int zeros = 0;
		unsigned int temp_num = line_number;
		while(temp_num!=0){
			zeros++;
			temp_num/=10;
		}
		for(int i=0; i<zeros; i++){
			temp_num=line_number;
			for(int j=0; j<i; j++){
				temp_num/=10;
			}
			temp_num%=10;
			error_string[i+strlen(invalid_syntax)]= temp_num + '0';
		}
		error_string[strlen(invalid_syntax)+zeros]='\n';
		free(invalid_syntax);
		break;
		
		Word_Error: ;
		issue++;
		char *invalid_words = "Error: Unrecognized arguments:  Line ";
		for(int i=0; i<strlen(invalid_words); i++){
			error_string[i]=invalid_words[i];
		}
		unsigned int zerosa = 0;
		unsigned int temp_numa = line_number;
		while(temp_numa!=0){
			zerosa++;
			temp_numa/=10;
		}
		for(int i=0; i<zerosa; i++){
			temp_numa=line_number;
			for(int j=0; j<i; j++){
				temp_numa/=10;
			}
			temp_numa%=10;
			error_string[i+strlen(invalid_words)]= temp_numa + '0';
		}
		error_string[strlen(invalid_words)+zeros]='\n';
		free(invalid_words);
	}/*End of While Loop (file loop)*/
	fclose(fp);
	return s;
}

/* 
 * This function writes the student list and the courses to a file in text format. 
 * The first line should contain a string:
 * "# Auto generated output file!"
 * The students are written in alphabetical order (dictionary order), 
 * and the courses are written in numerically increasing order. 
 * That is, higher numbered courses follow lower numbered courses. 
 */
 ERROR_CODE write_txt(char *filename, char *error_string){
	FILE *fp;
	struct student *q= NULL;
	struct course *c= NULL;
	q=s;
	fp=fopen(filename, "w");
	fprintf(fp,"# Auto generated output file!\n");
	fprintf(fp,"\n");
	while(q!=NULL){
		fprintf(fp,"student {\n");
		fprintf(fp,"\tname = "); 
		fprintf(fp,"%s\n",q->name); 
		fprintf(fp,"\tage  = "); 
		fprintf(fp,"%u\n",q->age); 
		fprintf(fp,"\tsex  = "); 
		fprintf(fp,"%c\n",q->sex);
		fprintf(fp,"\tgpa  =  "); 
		fprintf(fp,"%f\n",q->gpa); 
		c=q->courses;
		while(c!=NULL){
			fprintf(fp,"\tcourse {\n");
			fprintf(fp,"\t\tnumber = ");
			fprintf(fp,"%u\n",c->number); 
			fprintf(fp,"\t\tgrade  = "); 
			fprintf(fp,"%c\n",c->grade); 
			fprintf(fp,"\t}\n");
			c=c->next;
		}
		q=q->next;
		fprintf(fp,"}\n");
		fprintf(fp,"\n");
	}
	return 1;
}

/*
 * Bin format sample (also see sample file included using the hexdump program): 
 * 0xF0 0x0D
 * # students
 * student 1.name[32]
 * student 1.age
 * student 1.sex
 * student 1.gpa
 * number of courses for student 1
 * student1.course1.number
 * student1.course1.grade
 * student1.course2.number
 * student1.course2.grade
 * student 2.name[32]
 * ...
 * ...
 */
struct student *read_bin (char *filename, char *error_string){
	FILE *fp;
	fp = fopen(filename, "r");
	if(fp == NULL) {
		printf("Invalid Input\n");
		return 0;
	}
	
	unsigned int line_length = 0;
	unsigned int line_number = 0;
	struct student *head_student = (struct student *) malloc(0);
	s = NULL;
	
	char student_name[MAX_NAME_LENGTH];
	for(int i=0; i<MAX_NAME_LENGTH; i++){
		student_name[i]='\0';
	}
	unsigned int student_age;
	enum _SEX student_sex;
	float student_gpa;
	unsigned int course_number;
	unsigned char course_grade;
	
	/*these variables are used to determine if, at the current point in the file, the code is working within a student/course, and which student/course it is or would be working in should it be active*/
	unsigned int active_student = 0;
	unsigned int active_course = 0;
	short int issue = 0;
	
	char current_line[MAX_LINE_LENGTH+1];
	for(int i=0; i<MAX_LINE_LENGTH; i++){
		current_line[i]='\0';
	}
	fgets(current_line, MAX_LINE_LENGTH, fp);
	line_length = (unsigned int) strlen(current_line);
	line_number++;
	while(line_length > 0 && issue==0){
		char blank_word_1[MAX_NAME_LENGTH];
		char blank_word_2[MAX_NAME_LENGTH];
		char blank_word_3[MAX_NAME_LENGTH];
		char *words[3] = {blank_word_1, blank_word_2, blank_word_3};
		words[0][0]='\0';
		words[1][0]='\0';
		words[2][0]='\0';
		char current_word[MAX_NAME_LENGTH];
		for(int i=0; i<MAX_NAME_LENGTH; i++){
			current_word[i]='\0';
		}
		unsigned int word_index = 0;
		unsigned int char_index = 0;		
		for(int char_num=0; char_num<line_length; char_num++){
			char current_char = current_line[char_num];
			
			if(char_num==0 && current_char=='#'){
				for(int i=0; i<MAX_LINE_LENGTH; i++){
					current_line[i]='\0';
				}
				break;
			}
			if(current_char == ' ' || current_char == '\t' || current_char == '\n'){ /*If there's whitespace...*/
				if(char_index!=0){ /*... and an unsaved word...*/
					if(word_index>2){ /*... but not enough space to save the word,*/
						issue++;
						char *too_many_words = "Error: Too many words: Line ";
						for(int i=0; i<strlen(too_many_words); i++){
							error_string[i]=too_many_words[i];
						}
						unsigned int zeros = 0;
						unsigned int temp_num = line_number;
						while(temp_num!=0){
							zeros++;
							temp_num/=10;
						}
						for(int i=0; i<zeros; i++){
							temp_num=line_number;
							for(int j=0; j<i; j++){
								temp_num/=10;
							}
							temp_num%=10;
							error_string[i+strlen(too_many_words)]= temp_num + '0';
						}
						error_string[strlen(too_many_words)+zeros]='\n';
						free(too_many_words);
						break;
					} 
					else { /*... and there is enough space to save the word,*/
						strcpy(words[word_index], current_word);
						word_index++;
						for(int i=0; i<MAX_NAME_LENGTH; i++){
							current_word[i]='\0';
						}
						char_index = 0;
					}
				}
			}
			else{ /*If there isn't whitespace...*/
				current_word[char_index]=current_char;
				char_index++;
			}
			if(char_num==line_length-1){ /*If this is the last character of the line...*/
				if(char_index!=0){ /*... and there's an unsaved word...*/
					if(word_index>2){ /*... but there's already too many words,*/
						issue++;
						char *too_many_words = "Error: Too many words: Line ";
						for(int i=0; i<strlen(too_many_words); i++){
							error_string[i]=too_many_words[i];
						}
						unsigned int zeros = 0;
						unsigned int temp_num = line_number;
						while(temp_num!=0){
							zeros++;
							temp_num/=10;
						}
						for(int i=0; i<zeros; i++){
							temp_num=line_number;
							for(int j=0; j<i; j++){
								temp_num/=10;
							}
							temp_num%=10;
							error_string[i+strlen(too_many_words)]= temp_num + '0';
						}
						error_string[strlen(too_many_words)+zeros]='\n';
						free(too_many_words);
						break;
					} 
					else { /*... and there's room to save a new word,*/
						strcpy(words[word_index], current_word);
						for(int i=0; i<sizeof(*current_word); i++){
							current_word[i]='\0';
						}
						word_index++;
						char_index = 0;
					}
				}
			}
		}/*End of For Loop (line loop)*/
		if(!strcmp(words[0], "}")){
			if(active_course && active_student){
				active_course=0;
				add_course(student_name, course_number, course_grade);
			} else if (active_student){
				active_student=0;
			} else {
				goto Syntax_Error;
			}
		} else if(!strcmp(words[0], "student")){
		if(!strcmp(words[1], "{")){
				/*change the working student structure to fit with the given data*/
				if(!active_student && !active_course){
					active_student = 1;
					student_age = 0;
					student_gpa = 0.0;
					for(int i=0; i<MAX_NAME_LENGTH; i++){
						student_name[i]='\0';
					}
				} else {
					goto Syntax_Error;
				}
			} else {
				goto Word_Error;
			}
		} else if(!strcmp(words[0], "course")){
			if(!strcmp(words[1], "{")){
				/*change the working course structure within the working student structure to fit with the given data*/
				if(!active_course){
					active_course = 1;
					course_number = 0;
					course_grade = 'N';
					if(active_student==1){
						active_student=2;
						enroll_student(student_name, student_age, student_sex, student_gpa);
					}
				} else {
					goto Syntax_Error;
				}
			} else {
				goto Word_Error;
			}
		} else if(!strcmp(words[0], "name")){
			if(!strcmp(words[1], "=")){
				/*set the name field within the working student*/
				if(active_student){
					for(int i=0; i<strlen(words[2]); i++){
						student_name[i]=words[2][i];
					}
				} else {
					goto Syntax_Error;
				}
			} else {
				goto Word_Error;
			}
		} else if(!strcmp(words[0], "age")){
			if(!strcmp(words[1], "=")){
				/*set the age field within the working student*/
				if(active_student){
					student_age=atoi(words[2]);
				} else {
					goto Syntax_Error;
				}
			} else {
				goto Word_Error;
			}
		} else if(!strcmp(words[0], "sex")){
			if(!strcmp(words[1], "=")){
				/*set the sex field within the working student*/
				if(active_student){
					if(!strcmp(words[2], "M")){
						student_sex = 'M';
					} else if(!strcmp(words[2], "F")){
						student_sex = 'F';
					} else if(!strcmp(words[2], "O")){
						student_sex = 'O';
					} else {
						goto Word_Error;
					}
				} else {
					goto Syntax_Error;
				}
			} else {
				goto Word_Error;
			}
		} else if(!strcmp(words[0], "gpa")){
			if(!strcmp(words[1], "=")){
				/*set the gpa field within the working student*/
				if(active_student){
					student_gpa = (float) atof(words[2]);
				} else {
					goto Syntax_Error;
				}
			} else {
				goto Word_Error;
			}
		} else if(!strcmp(words[0], "number")){
			if(!strcmp(words[1], "=")){
				/*set the number field within the working course within the working student*/
				if(active_student && active_course){
					course_number = atoi(words[2]);
				} else {
					goto Syntax_Error;
				}
			} else {
				goto Word_Error;
			}
		} else if(!strcmp(words[0], "grade")){
			if(!strcmp(words[1], "=")){
				/*set the grade field within the working course within the working student*/
				if(active_student && active_course){
					course_grade = words[2][0];
				} else {
					goto Syntax_Error;
				}
			} else {
				goto Word_Error;
			}
		}
		for(int i=0; i<MAX_LINE_LENGTH; i++){
			current_line[i]='\0';
		}
		fgets(current_line, MAX_LINE_LENGTH, fp);
		line_length = (unsigned int) strlen(current_line);
		line_number++;
		continue;
		
		Syntax_Error: ;
		issue++;
		char *invalid_syntax = "Error: Invalid Syntax:  Line ";
		for(int i=0; i<strlen(invalid_syntax); i++){
			error_string[i]=invalid_syntax[i];
		}
		unsigned int zeros = 0;
		unsigned int temp_num = line_number;
		while(temp_num!=0){
			zeros++;
			temp_num/=10;
		}
		for(int i=0; i<zeros; i++){
			temp_num=line_number;
			for(int j=0; j<i; j++){
				temp_num/=10;
			}
			temp_num%=10;
			error_string[i+strlen(invalid_syntax)]= temp_num + '0';
		}
		error_string[strlen(invalid_syntax)+zeros]='\n';
		free(invalid_syntax);
		break;
		
		Word_Error: ;
		issue++;
		char *invalid_words = "Error: Unrecognized arguments:  Line ";
		for(int i=0; i<strlen(invalid_words); i++){
			error_string[i]=invalid_words[i];
		}
		unsigned int zerosa = 0;
		unsigned int temp_numa = line_number;
		while(temp_numa!=0){
			zerosa++;
			temp_numa/=10;
		}
		for(int i=0; i<zerosa; i++){
			temp_numa=line_number;
			for(int j=0; j<i; j++){
				temp_numa/=10;
			}
			temp_numa%=10;
			error_string[i+strlen(invalid_words)]= temp_numa + '0';
		}
		error_string[strlen(invalid_words)+zeros]='\n';
		free(invalid_words);
	}/*End of While Loop (file loop)*/
	fclose(fp);
	return s;
}

/* 
 * This function writes the student list and the courses to a file in binary format. 
 * The first two bytes must be 0xF0 and 0x0D.
 * The students are written in alphabetical order (dictionary order), 
 * and the courses are written in numerically increasing order. 
 * That is, higher numbered courses follow lower numbered courses. 
 * SUCCESS or FAILURE is appropriately returned.
 * The error string must contain an informative error message. 
 */
ERROR_CODE write_bin(char *filename, char *error_string){
	FILE *fp;
	struct student *q= NULL;
	struct course *c= NULL;
	q=s;
	fp=fopen(filename, "wb+");
	fprintf(fp,"# Auto generated output file!\n");
	fprintf(fp,"\n");
	while(q!=NULL){
		fprintf(fp,"student {\n");
		fprintf(fp,"\tname = "); 
		fprintf(fp,"%s\n",q->name); 
		fprintf(fp,"\tage  = "); 
		fprintf(fp,"%ui\n",q->age); 
		fprintf(fp,"\tsex  = "); 
		fprintf(fp,"%c\n",q->sex);
		fprintf(fp,"\tgpa  =  "); 
		fprintf(fp,"%f\n",q->gpa); 
		c=q->courses;
		while(c!=NULL){
			fprintf(fp,"\tcourse {\n");
			fprintf(fp,"\t\tnumber = ");
			fprintf(fp,"%ui\n",c->number); 
			fprintf(fp,"\t\tgrade  = "); 
			fprintf(fp,"%ui\n",c->grade); 
			fprintf(fp,"\t}\n");
			c=c->next;
		}
		q=q->next;
		fprintf(fp,"}\n");
		fprintf(fp,"\n");
	}
	return 1;
}

/*
 * This functions create a new student and store it in its appropriate position (following dictionary order)
 * in the list. A pointer to the newly added student structure is returned. 
 */
struct student* enroll_student(char *name, unsigned int age, SEX sex, float gpa){

	struct student *p= NULL;
	struct student *q= NULL;
	struct student *t=NULL;
	p=s;
	if(s==NULL){
		t = malloc(sizeof(struct student));
			for(int x=0; x<MAX_NAME_LEN; x++){
				t->name[x]=name[x];
				if(name[x]=='\0'){
					break;
				}
				
			}
			t->age=age;
			t->sex=sex;
			t->gpa=gpa;
			t->next=s;
			s=t;
			return t;
	}
	for(int i=0; i<MAX_NAME_LEN; i++){
		if(p->name[i]>name[i]){
			t = malloc(sizeof(struct student));
			for(int x=0; x<MAX_NAME_LEN; x++){
				t->name[x]=name[x];
				if(name[x]=='\0'){
					break;
				}
				
			}
			t->age=age;
			t->sex=sex;
			t->gpa=gpa;
			t->next=s;
			s=t;
			return t;
		}
		else if(p->name[i]=='\0'){
			break;
		}
		else if(name[i]=='\0'){
			printf("%i\n",i);
			t = malloc(sizeof(struct student));
			for(int x=0; x<MAX_NAME_LEN; x++){
				t->name[x]=name[x];
				if(name[x]=='\0'){
					break;
				}
			}
			t->age=age;
			t->sex=sex;
			t->gpa=gpa;
			t->next=s;
			s=t;
			return t;
		}
	}
	
	q=s;
	p=q->next;
	while(p!=NULL){
		for(int i=0; i<MAX_NAME_LEN; i++){
			if(p->name[i]>name[i]){
				t = malloc(sizeof(struct student));
				q->next=t;
				for(int x=0; x<MAX_NAME_LEN; x++){
					t->name[x]=name[x];
					if(name[x]=='\0'){
						break;
					}
				}
				t->age=age;
				t->sex=sex;
				t->gpa=gpa;
				t->next=p;
				return t;
			}
			else if(p->name[i]=='\0'){
				break;
			}
			else if(name[i]=='\0'){
				t = malloc(sizeof(struct student));
				q->next=t;
				for(int x=0; x<MAX_NAME_LEN; x++){
					t->name[x]=name[x];
					if(name[x]=='\0'){
						break;
					}
				}
				t->age=age;
				t->sex=sex;
				t->gpa=gpa;
				t->next=p;
				return t;
			}
		}
		q=p;
		p=p->next;
	}
	return NULL;
}

/*
 * This function removes a student given the name (case sensitive).
 * A pointer to the removed student structure is returned. 
 */
struct student* remove_student(char *name){
	struct student *p= NULL;
	struct student *q= NULL;
	p=s;
	q=p;
	int i=0;
	while(name[i]!='\0' && p->name[i]!='\0'){
		if(p->name[i]!=name[i]){
			break;
		}
		i++;
		if(name[i]=='\0' && p->name[i]=='\0'){
			s= p->next;
			return q;
		}
	}
	while(p->next!=NULL){
		q=p;
		p=p->next;
		i=0;
		while(name[i]!='\0' && p->name[i]!='\0'){
			if(p->name[i]!=name[i]){
				break;
			}
			i++;
			if(name[i]=='\0' && p->name[i]=='\0'){
				struct student *z= NULL;
				z=q->next;
				q->next=p->next;
				return z;
			}
	}}
	return NULL;
}

/*
 * This function adds a course information for a student. 
 * A course structure is created and added at appropriate position in the courses list within student structure. 
 * A pointer to the newly added course is returned.  
 */
struct course* add_course(char *name, unsigned int number, char grade){
	struct student *p= NULL;
	struct student *q= NULL;
	struct course *c= NULL;
	struct course *v= NULL;
	struct course *t= NULL;
	for(int i=0; i<MAX_NAME_LEN; i++){
		if(s->name[i]!=name[i]){
			break;
		}
		//printf("%d b\n",i);
		if(s->name[i]=='\0' && name[i]=='\0'){
			c=s->courses;
			if(c==NULL){
				t = malloc(sizeof(struct course));
				t->number=number;
				t->grade=grade;
				t->next=c;
				s->courses=t;
				return t;
			}
				

			if(c->number>number){
					t = malloc(sizeof(struct course));
					t->number=number;
					t->grade=grade;
					t->next=c;
					s->courses=t;
					return t;
					
			}
			v=c;
			c=c->next;
			while(c!=NULL){
				if(c->number>number){
					t = malloc(sizeof(struct course));
					t->number=number;
					t->grade=grade;
					t->next=c;
					v->next=t;
					return t;
				}
				v=c;
				c=c->next;
			}
		}
	}
	
	q=s;
	p=q->next;
	while(p!=NULL){
		for(int i=0; i<MAX_NAME_LEN; i++){
			if(p->name[i]!=name[i]){
				break;
			}
			if(p->name[i]=='\0' && name[i]=='\0'){
				c=p->courses;
				if(c->number>number){
						t = malloc(sizeof(struct course));
						t->number=number;
						t->grade=grade;
						t->next=c;
						p->courses=t;
						return t;
					}
				v=c;
				c=c->next;
				while(c!=NULL){
					if(c->number>number){
						t = malloc(sizeof(struct course));
						t->number=number;
						t->grade=grade;
						t->next=c;
						v->next=t;
						return t;
					}
					v=c;
					c=c->next;
				}
				
			}			
		}
		q=p;
		p=p->next;
	}
	return NULL;
}

/*
 * This function removes a course structure from a student's course list given the student's name. 
 * A pointer to the removed structure is returned. 
 */
struct course* remove_course(char *name, unsigned int number){
	struct student *p= NULL;
	struct student *q= NULL;
	struct course *c= NULL;
	struct course *v= NULL;
	struct course *t= NULL;
	for(int i=0; i<MAX_NAME_LEN; i++){
		if(s->name[i]!=name[i]){
			break;
		}
		if(s->name[i]=='\0' && name[i]=='\0'){
			c=s->courses;
			if(c->number==number){
					if(c->next==NULL){
						s->courses=NULL;
						return c;
					}
					
					s->courses=c->next;
					return c;
				}
			v=c;
			c=c->next;
			while(c!=NULL){
				if(c->number>number){
					if(c->next==NULL){
						v->next=NULL;
						return c;
					}
					
					v->next=c->next;
					return c;
				}
			}
		}
	}
	
	q=s;
	p=q->next;
	while(p!=NULL){
		for(int i=0; i<MAX_NAME_LEN; i++){
			if(p->name[i]!=name[i]){
				break;
			}
			if(p->name[i]=='\0' && name[i]=='\0'){
				c=p->courses;
				if(c->number==number){
					if(c->next==NULL){
						p->courses=NULL;
						return c;
					}
					
					p->courses=c->next;
					return c;
				}
				v=c;
				c=c->next;
				while(c!=NULL){
					if(c->number>number){
						if(c->next==NULL){
							v->next=NULL;
							return c;
						}
					
						v->next=c->next;
						return c;
					}
				}
			}			
		}
		q=p;
		p=p->next;
	}
	return NULL;
}

/*
 * This function systematically frees all the memory. 
 * NOTE: All the memory must be freed (i.e., all students and all courses). There must be no memory leak. 
 */ 
void delete_all(){
	struct course *c=NULL;
	struct student *a=NULL;
	struct course *d=NULL;
	while(s!=NULL){
		c=s->courses;
		while(c!=NULL){
			d=c->next;
			free(c);
			c=d;
		}
		a=s->next;
		free(s);
		s=a;
	}
}

/*
 * This function lists the names of all students in the list (in alphabetical order). 
 */
void list_students(){
	struct student *p= NULL;
	if(s!=NULL){
		printf("%s",s->name);
		p=s->next;
	}
	
	while(p!=NULL){
		printf("%s",p->name);
		p=p->next;
	}
}

/*
 * This function lists all the course numbers enrolled by all the students. 
 * For example, if student 1 is registered for 120, 220, 240 and 440, and 
 * student 2 is registered for 120, 340 and 580. This function prints out:
 * 120, 220, 240, 340, 440 and 580. 
 */
void list_courses(){
	unsigned int i=0;
	unsigned int e=0;
	struct student *p= NULL;
	struct course *c= NULL;
	/*if(s!=NULL){
		c=s->courses;
		while(c!=NULL){
			if(c->number > i){
				printf("%u",c->number);
				i=c->number;
			}
			c=c->next;
		}
		p=s->next;
	}*/
	p=s;
	while(p!=NULL){
		c=p->courses;
		while(c!=NULL){
			/*if(c->number){
				printf("%u",c->number);
				i=c->number;
			}*/
			i++;
			c=c->next;
		}
		p=p->next;
	}
	unsigned int j[i];
	p=s;
	i=i-1;
	e=i;
	while(p!=NULL){
		c=p->courses;
		while(c!=NULL){
			/*if(c->number){
				printf("%u",c->number);
				i=c->number;
			}*/
			while(e>=0){
				if(j[e]==c->number){
					break;
				}
				if(e==0){
					printf("%u",c->number);
					j[i+1]=c->number;
					i++;
				}
			}
			e=i;
			c=c->next;
		}
		p=p->next;
	}
}