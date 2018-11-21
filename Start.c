#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#define def_iface "eth0"


/*GLOBAL VARIABLES*/
	char *ip_ad,ip_ad_search[20];
	char is_right_command_entered=0;


	/*GET STRING BEFORE SYMBOL "|" IN LINE*/
	char * get_str_before_sym_from_certain_line_of_file(char filename[100], unsigned int line_number){

		FILE *file;
		char bash_cmd[200];
		char line_number_str[10];
		char result_line[100];
		short fisrt_symbol_after_ip;
		char ip[20];
		short first_symbol_after_ip;
		static char * ip_ad;


		strcpy(bash_cmd,"cat ");
		strcat(bash_cmd,filename);
		strcat(bash_cmd,"|head -n ");
		sprintf(line_number_str, "%d", line_number);
		strcat(bash_cmd,line_number_str);
		strcat(bash_cmd," | tail -n 1");

		file = popen(bash_cmd,"r");
		fgets(result_line,sizeof(result_line),file);
		fclose(file);
	
	
		first_symbol_after_ip = strchr(result_line,'|')-result_line;
		for (int i=0;i<first_symbol_after_ip;i++) ip[i]=result_line[i];
		ip[first_symbol_after_ip]='\0';
	
		//char *ptr = malloc(sizeof(char) * 20);
		ip_ad=ip;
		sprintf(ip_ad_search,"%s",ip);
		//ip_ad_search=ip;
		return (ip_ad);


	}



	/*GET NUMBER AFTER SYMBOL "|" IN LINE*/
	unsigned long get_num_after_sym_from_certain_line_of_file(char filename[100], unsigned int line_number){

		FILE *file;
		char bash_cmd[200];
		char line_number_str[10];
		char result_line[100];
		short first_symbol_before_amount;

		char package_amount_str[20];
		unsigned long package_amount=0;

		strcpy(bash_cmd,"cat ");
		strcat(bash_cmd,filename);
		strcat(bash_cmd,"|head -n ");
		sprintf(line_number_str, "%d", line_number);
		strcat(bash_cmd,line_number_str);
		strcat(bash_cmd," | tail -n 1");

		file = popen(bash_cmd,"r");
		fgets(result_line,sizeof(result_line),file);
		fclose(file);

		first_symbol_before_amount = strchr(result_line,'|')-result_line;
		strtok(result_line, "\n");
		short ctr=0;

		for (int i=(first_symbol_before_amount+1);i<strlen(result_line);i++) package_amount_str[ctr++]=result_line[i];

		package_amount_str[ctr]='\0';


		for (int j=0;j<(strlen(result_line)-first_symbol_before_amount-1);j++){
		
			package_amount*=10;		
			package_amount+=package_amount_str[j]-'0';
		

		}
	
		return (package_amount);


	}


	/*COMPARING IP ADDRESSES (FOR SORTING AND POP)*/
	/* "1" - ip1>ip2     "2" - ip1<ip2   "3" - ip1==ip2  */

	int compare_ip_addr(char ip1[20],char ip2[20]){


	
		int min_length_of_ip;

		if (strlen(ip1)<=strlen(ip2)) min_length_of_ip=strlen(ip1);
		else min_length_of_ip=strlen(ip2);

		for (int i=0;i<min_length_of_ip;i++){
		
			if ((int)ip1[i]>(int)ip2[i]) return 1;
			if ((int)ip1[i]<(int)ip2[i]) return 2;
			if ((int)ip1[i]==(int)ip2[i]) continue;
		}
		if (strlen(ip1)>strlen(ip2)) return 1;
		if (strlen(ip1)<strlen(ip2)) return 2;
		return 3;
	}







	/*GET OUTPUT OF SHELL*/
	/*char* Checking_for_shell_output(char *command,char *probably_str)
		{	
			FILE *fp;
			char str[200];
			char *result;
		
			fp = popen(command,"r"); 
			fgets(str,sizeof str,fp); 		
			result = strstr(str,probably_str);		
	     		pclose(fp);	
		
			return (result);
		}*/




	/*GET AMOUNT OF LINES OF FILE aka "WC -l"*/
	unsigned long get_number_of_lines_in_file(char filepath[]){
			FILE *file;
			char cmd_output[100];
			char command[200];
			char *ptr;
			unsigned long number=0;
			strcpy(command,"wc -l ");
			strcat(command,filepath);
			file = popen(command,"r");
			fgets(cmd_output,sizeof(cmd_output),file);
		
			pclose(file);
			ptr = strstr(cmd_output,filepath);
		
			/*String to unsigned long*/
			for (short i=0;i<(ptr-cmd_output-1);i++){
				number*=10;
				number+=cmd_output[i]-'0';
			
			}
		
			return (number);
		
		}




	/*RETUR ORDER OF CURRENT USED IFACE */
	short return_order_of_iface(char *iface){
	

		//FILE *iface_order;
		//iface_order = fopen("iface_order","r");
		short order=1;
		char order_str[5];
		char buffer[30];
		char command[50];
		unsigned long amount_lines=get_number_of_lines_in_file("iface_order");	
		if (amount_lines==0){
		strcpy(command,"echo \"");
		sprintf(order_str,"%d",order);
		strcat(command,order_str);
		strcat(command,iface);
		strcat(command,"|0\" >> iface_order");
		order=amount_lines;
		system(command);
		return 1;
		}			
		printf("''%s''",iface);
		for (int i=0; i<amount_lines;i++){
			sprintf(buffer,"%d%s",i+1,iface);
			/*printf("||%d||",strcmp(buffer,get_str_before_sym_from_certain_line_of_file("iface_order",i+1)));
			printf("--%s|%s--",buffer,get_str_before_sym_from_certain_line_of_file("iface_order",i+1));*/
			printf("--%s|%s--",buffer,get_str_before_sym_from_certain_line_of_file("iface_order",i+1));
			if(strcmp(buffer,get_str_before_sym_from_certain_line_of_file("iface_order",i+1))==0)  return (i+1);
			
		}
		
		
		strcpy(command,"echo \"");
		sprintf(order_str,"%lu",amount_lines+1);
		strcat(command,order_str);
		strcat(command,iface);
		strcat(command,"|0\" >> iface_order");
		printf("!--%s--!",command);
		system(command);
		
		return amount_lines+1;

	}




	

	/*INSERTING UDATED INFO TO SORTED ARRAY IN FILE "ipstat"*/

	void Insert_ip_packages_in_stat_file(char filename[100],unsigned long position,char ip[20],int num_of_new_packages,int is_new_ip_addr){
	
		system("touch ipstat_copy");
		char bash_cmd[500];
		unsigned long max_line=get_number_of_lines_in_file(filename);
		if (is_new_ip_addr==0){
			sprintf(bash_cmd,"touch ipstat_copy; cat %s| head -n %lu > ipstat_copy; echo \"%s|%lu\" >> ipstat_copy ; cat %s |tail -n %lu >> ipstat_copy; cat ipstat_copy > ipstat; rm ipstat_copy",filename,position-1,ip,get_num_after_sym_from_certain_line_of_file(filename,position)+num_of_new_packages,filename,max_line-position);


		
		
			system(bash_cmd);
		
			system("cat ipstat");
		}
		 else {

			sprintf(bash_cmd,"touch ipstat_copy; cat %s| head -n %lu > ipstat_copy; echo \"%s|%d\" >> ipstat_copy ; cat %s |tail -n %lu >> ipstat_copy; cat ipstat_copy > ipstat; rm ipstat_copy",filename,position-1,ip,num_of_new_packages,filename,max_line-position+1);
		
			system(bash_cmd);
		
			system("cat ipstat");
		}

	}







	/*PREPARATION BEFORE INSERTING UDATED INFO TO SORTED ARRAY IN FILE "ipstat"*/
	int binarysearch_insert_new_info_to_ip_stat(char ip[20], int num_of_new_packages)
	{

		unsigned long num_of_ip=get_number_of_lines_in_file("ipstat");
	
		char bash_cmd[200];
		unsigned int low, high, middle;
		unsigned long new_position=0;
		char ip_address[20];
		int is_new_ip_addr=1;


		if (num_of_ip==0) {
			strcpy(bash_cmd,"echo \"");
			strcat(bash_cmd,ip);
			strcat(bash_cmd,"|");
			char str[10];
			sprintf(str, "%d", num_of_new_packages);
			strcat(bash_cmd,str);
			strcat(bash_cmd,"\" >> ipstat");
			system(bash_cmd);
	
			system("cat ipstat");
			return(0);
		}
	    
		low = 1;
		high = num_of_ip ;
	
		int j=0;
		while (low <= high){
			j++;
	
			middle = (low + high) / 2;
	
			ip_ad=get_str_before_sym_from_certain_line_of_file("ipstat",middle);
			sprintf(ip_address,"%s",ip_ad);
	
	
			if (compare_ip_addr(ip,ip_address)==1) low = middle + 1;
			else if (compare_ip_addr(ip,ip_address)==2) high = middle - 1;
			else {
		
				new_position = middle;
				int is_new_ip_addr=0;
				Insert_ip_packages_in_stat_file("ipstat",new_position,ip,num_of_new_packages, is_new_ip_addr);
				return (1);	
			}

			/*if (ip < mass[middle])
			    high = middle - 1;
			else if (a > mass[middle])
			    low = middle + 1;
			else 
			    return middle;*/
	
		}
		if (low>high) new_position = low;
		else new_position = high;
	
		Insert_ip_packages_in_stat_file("ipstat",new_position,ip,num_of_new_packages, is_new_ip_addr);
	
	    	return (1);
	}





	/*SEARCH IP STATISTIC IN FILE "ipstat"*/
	long binarysearch_log_N(char ip[20])
	{

		unsigned long num_of_ip=get_number_of_lines_in_file("ipstat");
	
		char bash_cmd[200];
		unsigned int low, high, middle;
		unsigned long position=0;
		char *ip_address;
	


		if (num_of_ip==0) {
		return (-1);
		}
	    
	    	low = 1;
	    	high = num_of_ip ;
	
		int j=0;
	    	while (low <= high){
			j++;
	
			middle = (low + high) / 2;
	
			ip_address=get_str_before_sym_from_certain_line_of_file("ipstat",middle);
			sprintf(ip_address,"%s",ip_ad_search);
	
	
			if (compare_ip_addr(ip,ip_ad_search)==1) low = middle + 1;
			else if (compare_ip_addr(ip,ip_ad_search)==2) high = middle - 1;
			else {
		
			    	position = middle;
	
				return (position);	
			}

			/*if (ip < mass[middle])
			    high = middle - 1;
			else if (a > mass[middle])
			    low = middle + 1;
			else 
			    return middle;*/
	
	    	}
	    
	
	    	return (-1);
	}





	/*COPY FILTERED LOGS FROM FILE "syslog"*/
	unsigned long copy_logs_from_syslog_to_file(unsigned long syslog_last_coppied_line, char *grep_filter_sentence){
			char command[200];
			char string_converted_from_number[20];
			unsigned long new_syslog_last_coppied_line;
		
			strcpy(command,"sed -n ' ");
			sprintf(string_converted_from_number, "%lu", syslog_last_coppied_line);
			//lltoa(syslog_last_coppied_line,string_converted_from_number,10);
			strcat(command,string_converted_from_number);
			new_syslog_last_coppied_line=get_number_of_lines_in_file("/var/log/syslog");
		
			if (syslog_last_coppied_line==new_syslog_last_coppied_line) return(syslog_last_coppied_line);

			sprintf(string_converted_from_number, "%lu", new_syslog_last_coppied_line);
			//lltoa(new_syslog_last_coppied_line,string_converted_from_number,10);
			strcat(command,",");
			strcat(command,string_converted_from_number);
			strcat(command,"p' /var/log/syslog | grep ");
			strcat(command,grep_filter_sentence);
			strcat(command," >> prog.log ");
		
			system(command);
			return (new_syslog_last_coppied_line);
		}


	/*UPLOAD STATISTIC TO FILE "iface_order"*/
	void upload_iface_statistic(char *iface,unsigned int package_num){
		FILE *iface_order;
		char bash_cmd[200];
		char result_line[100];
		unsigned int line_num;
		char *iface_name;
		unsigned long max_line_num;
		unsigned long old_num_of_packages=0,new_num_of_packages=0;
		char *iface_declaration;
		char new_num_of_packages_str[20];
		int i=1;
		char buffer[100];
		char str[20];
	
		max_line_num=get_number_of_lines_in_file("iface_order");
		
		for (int j=0; j<max_line_num;j++){
				sprintf(buffer,"%d%s",j+1,iface);
				iface_name=get_str_before_sym_from_certain_line_of_file("iface_order",j+1);
				old_num_of_packages = get_num_after_sym_from_certain_line_of_file("iface_order",j+1);
				if(strcmp(iface_name,buffer)==0){ 
					new_num_of_packages=old_num_of_packages+package_num;
	
	
					sprintf(bash_cmd,"touch iface_copy; head -n %d iface_order > iface_copy; echo \"%d%s|%lu\" >> iface_copy; tails -n %lu iface_order >> iface_copy; cat iface_copy > iface_order; rm iface_copy",j,j+1,iface,new_num_of_packages,max_line_num-j+1);
					printf("--%s--",bash_cmd);
					system(bash_cmd);}
		}
			
		

		/*iface_order = fopen("iface_order","r");
		while (fgets(result_line,sizeof(result_line),iface_order)){
			 if (strstr(result_line,iface)!=NULL) line_num=i;
			i++;
		}
		/*for (int i=0; i<max_line_num;i++){
			fgets(result_line,sizeof(result_line),iface_order);
			if (strstr(result_line,iface)!=NULL) line_num=i+1;
		}*/
		//fclose(iface_order);*/
	
		
		


	}




	/*GREP STATISTIC FROM FILE "prog.log"*/
	/*read "prog.log" and get ip and number of packets to file "temporary_ip_list"*/
	void read_logs(char *iface){
		FILE *logs;
		char buffer[1000];
		char ip_list[get_number_of_lines_in_file("prog.log")][16];
		char count_of_packets[get_number_of_lines_in_file("prog.log")];
		char *ptr_in;
		char *ptr_out;
		char *ptr_src;
		char *ptr_dst;
		char interface[10]="";
		char source_ip[16]="";
		short iface_symbol_counter=0;
		short ip_symbol_counter=0;
		unsigned int amount_of_new_ip=0;
		short is_ip_new=1;
		char command[100];
		char str[5];
		unsigned int sum_of_packets=0;
	

	
		system("cp prog.log temporary.log");
		system("chmod 777 temporary.log");
	
	
		logs=fopen("temporary.log","r");
	
		int cou=0;
		while (fgets(buffer,sizeof(buffer),logs)){
		
			iface_symbol_counter=0;
			ip_symbol_counter=0;
		
			ptr_in = strstr(buffer,"IN=");	
			ptr_out = strstr(buffer,"OUT=");
			ptr_src = strstr(buffer,"SRC=");
			ptr_dst = strstr(buffer,"DST=");
		
			for ( long i=ptr_in-buffer+3; i<(ptr_out-buffer-1);i++){
			
			
				interface[iface_symbol_counter]=buffer[i];
				iface_symbol_counter++;
			}
		
			interface[iface_symbol_counter]='\0';
			strcpy(source_ip,"");		

			for ( long j=ptr_src-buffer+4; j<(ptr_dst-buffer-1);j++){
			
				source_ip[ip_symbol_counter]=buffer[j];
				ip_symbol_counter++;
			}

			source_ip[ip_symbol_counter]='\0';
		
		
		
			if (strcmp(iface,interface)==0) {
			
				strcpy(ip_list[amount_of_new_ip],source_ip);
				amount_of_new_ip+=1;
			
				
			}	
		
	
		
			
	
	
		}
		fclose(logs);
		system("rm temporary.log");
	
	
		system("touch temporary_ip_list; chmod 777 temporary_ip_list");
	
	

		char unique_ip_list[amount_of_new_ip][16];
		unsigned int amount_of_new_unique_ip=0;
		unsigned int number_of_packets[amount_of_new_ip];
		short is_find_ip=0;	
	
		for (int l=0;l<amount_of_new_ip;l++){
			for (int m=0;m<amount_of_new_unique_ip;m++){
			
				if (strcmp(ip_list[l],unique_ip_list[m])==0) {
					number_of_packets[m]+=1;
					is_find_ip=1;
				}
			}
			if (is_find_ip==0) {
				strcpy(unique_ip_list[amount_of_new_unique_ip],ip_list[l]);
				number_of_packets[amount_of_new_unique_ip]=1;	
				amount_of_new_unique_ip++;
				
			}
			is_find_ip=0;
		}
	
		sum_of_packets=0;
		for (int o=0; o<amount_of_new_unique_ip;o++) {
		
			strcpy(command,"echo \"");
			strcat(command,unique_ip_list[o]);
			strcat(command,"|");
			char str[10];
			sprintf(str, "%d", number_of_packets[o]);
			strcat(command,str);
			strcat(command,"\" >> temporary_ip_list");
			system(command);
		
			int success=binarysearch_insert_new_info_to_ip_stat(unique_ip_list[o],number_of_packets[o]);
			sum_of_packets+=number_of_packets[o];

		}

		//upload_iface_statistic(iface,sum_of_packets);
		system("rm temporary_ip_list");
		system("> prog.log");
	
	}



	void Create_functional_files(){
		system("touch current_iface iface_order ipstat prog.log PID");
	}

	/*CONSOLE COMMAND FUNCTIONS*/

	void Start(){
		char cmd[500];
	
		if (get_number_of_lines_in_file("current_iface")==0){
		strcpy(cmd,"iptables -I INPUT -i ");
		strcat(cmd,def_iface);
		strcat(cmd," -j LOG --log-prefix \"Iptables_sniffer: \"");
	
		system(cmd);
	
		system("touch current_iface");
		system("touch iface_order");
		strcpy(cmd,"echo \"");
		strcat(cmd,def_iface);
		strcat(cmd,"\" > current_iface");
		system(cmd);
		printf("\nStart sniffing interface DEFAULT IFACE\n");
		}
		else printf("\nWarning: Sniffing already started\n");
		is_right_command_entered=1;
	}






	void Stop (){
		char cmd[500];
		FILE *current_iface;
		char iface[20];	
		current_iface = fopen("current_iface","r");
		fgets(iface,sizeof(iface),current_iface);
		fclose(current_iface);
		strtok(iface, "\n");
		strcpy(cmd,"iptables -D INPUT -i ");
		strcat(cmd,iface);
		strcat(cmd," -j LOG --log-prefix \"Iptables_sniffer");
		strcat(cmd,": \"");
	
		system(cmd);
		system("> current_iface");
		printf("\nSniffing stopped\n");
		is_right_command_entered=1;
	}






	char* Select_iface (char new_iface[20]){
		char cmd[500];
		FILE *current_iface;
		char previous_iface[20];
		unsigned int lines_of_current_iface_file;	
	
		lines_of_current_iface_file=get_number_of_lines_in_file("current_iface");
		if (lines_of_current_iface_file!=0) {
		
			current_iface = fopen("current_iface","r");
			fgets(previous_iface,sizeof(previous_iface),current_iface);
			fclose(current_iface);
		
	
			strcpy(cmd,"iptables -D INPUT -i ");
			strtok(previous_iface, "\n");
			strcat(cmd,previous_iface);
			strcat(cmd," -j LOG --log-prefix \"Iptables_sniffer");
			strcat(cmd,": \"");
		
			system(cmd);
		}

		strcpy(cmd,"iptables -I INPUT -i ");
		strcat(cmd,new_iface);
		strcat(cmd," -j LOG --log-prefix \"Iptables_sniffer");
		strcat(cmd,": \"");
	
		system(cmd);
	
		system("touch current_iface");
		strcpy(cmd,"echo \"");
		strcat(cmd,new_iface);
		strcat(cmd,"\" > current_iface");
		system(cmd);
		is_right_command_entered=1;
		return (new_iface);
	}



	void Show_ip_statistic(char ip[20]){

	 	long line_num;
		unsigned long number_of_packets=0;

		line_num = binarysearch_log_N(ip);
		if (line_num==-1) printf("\nNo information about this ip\n");
		else{
			number_of_packets=get_num_after_sym_from_certain_line_of_file("ipstat",line_num);
			printf("    Recieved %lu packets from %s \n",number_of_packets,ip);
		};
	is_right_command_entered=1;
	}


	void Show_iface_statistic(char iface[20]){

	 	unsigned long line_amount;
		unsigned long number_of_packets=0;
		char *iface_name;
		short is_find_iface=0;
		char buffer[50];
		
		printf("\n%s\n",iface);
		line_amount = get_number_of_lines_in_file("iface_order");
		if (line_amount==0) {printf("No statistic"); is_find_iface=1;}
		if (iface==NULL) {
			for (int i=0; i<line_amount;i++) {
				
				iface_name=get_str_before_sym_from_certain_line_of_file("iface_order",i+1);
				number_of_packets = get_num_after_sym_from_certain_line_of_file("iface_order",i+1);
				iface_name[0]=' ';
				printf("\n     Recieved %lu packets from %s interface\n",number_of_packets,iface_name);
			}
		}
		else {
			for (int j=0; j<line_amount;j++){
				sprintf(buffer,"%d%s",j+1,iface);
				iface_name=get_str_before_sym_from_certain_line_of_file("iface_order",j+1);
				number_of_packets = get_num_after_sym_from_certain_line_of_file("iface_order",j+1);
				if(strcmp(iface_name,buffer)==0)	{ iface_name[0]=' '; printf("\n     Recieved %lu packets from %s interface\n",number_of_packets,iface_name);	is_find_iface=1;}
			}
			if (is_find_iface==0) printf("No statistic about this interface\n");
		}
		is_right_command_entered=1;
	}


	int main(int argc, char *argv[])
	{

		char *iface;
		short iface_order_in_stat_file=0;
		FILE *fp;
		FILE *syslog;
		unsigned long syslog_counter=0;
		char str[200];
		char buffer [1000];
		char log_prefix[200]="Iptables_sniffer";
		char PID[20];	

		pid_t process_id = 0;
		pid_t sid = 0;


		Create_functional_files();
	


		/*CONSOLE COMMAND HANDLING*/
		if (argc<2){
			printf("Use attribute \"--help\"\n");
			return -1;
			}
	
		//if (strcmp(argv[1],"start")==0) printf("yes,it start");
	
		if ((strcmp(argv[1],"stop")==0)&&(argc==2)) Stop(); 

		//if ((strcmp(argv[1],"select")==0)&&(strcmp(argv[2],"iface")==0)&&(argc==3)) iface=Select_iface(def_iface); 

		if ((strcmp(argv[1],"select")==0)&&(strcmp(argv[2],"iface")==0)&&(argc==4)) {

			iface=Select_iface(argv[3]);
			iface_order_in_stat_file=return_order_of_iface(iface); 
		}


		if ((strcmp(argv[1],"show")==0)&&(strcmp(argv[3],"count")==0)&&(argc==4)) Show_ip_statistic(argv[2]);

	
		if ((strcmp(argv[1],"stat")==0)&&(argc<=3)) Show_iface_statistic(argv[2]);

		if ((strcmp(argv[1],"start")==0)&&(argc==2))
		{	
			//iface = "eth0";
			Start();
			iface = def_iface;
			iface_order_in_stat_file=return_order_of_iface(iface);
		}
	
		if ((strcmp(argv[1],"--help")==0)&&(argc==2))
		{
			printf("\n\"start\"​: packets are being sniffed from now on from default iface(eth0)\n\n");
			printf("\"stop\": packets are not sniffed\n\n");
			printf("\"show [ip] count\": show [ip] count ​print number of packets received from ip address\n\n");
			printf("\"select iface [iface]\": select interface for sniffing eth0, wlan0, ethN,wlanN...\n\n");
			printf("\"stat​ [iface]​\": show all collected statistics for particular interface, if iface omitted - for all interfaces\n\n");
			printf("\"--help\": show usage information\n\n");
			is_right_command_entered=1;
		}



		if (is_right_command_entered==0) printf("Wrong command!\nUse \"--help\" attribute to see manual...\n");
		/*DEAMON DECLARATIONS*/
		
		process_id = fork();
		// Indication of fork() failure
		if (process_id < 0)
		{
		printf("fork failed!\n");
		// Return failure in exit status
		exit(1);
		}
		// PARENT PROCESS. Need to kill it.
		if (process_id > 0)
		{
		printf("process_id of child process %d \n", process_id);
		// return success in exit status
		char PID[100];
		sprintf(PID,"echo \"%d\" > PID",process_id);
		system(PID);
		exit(0);
		}
		//unmask the file mode
		umask(0);
		//set new session
		sid = setsid();
		if(sid < 0)
		{
		// Return failure
		exit(1);
		}
		// Change the current working directory to root.
		
		// Close stdin. stdout and stderr
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);

		/*COLECTING STATISTIC*/
		syslog_counter = get_number_of_lines_in_file("/var/log/syslog");
	

		while(1){						
			syslog_counter=copy_logs_from_syslog_to_file(syslog_counter,"Iptables_sniffer:");
			read_logs(iface);
			sleep(10);			
		}
	
		return 0;
	}

