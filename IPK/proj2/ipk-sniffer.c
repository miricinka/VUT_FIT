/* ==========================================================================*/
/*                                                                           */
/*   Nazev programu: ipk-sniffer.c                                           */
/*   Popis programu: Sitovy analyzator, zachycuje a filtruje pakety          */
/*   Autor:          Mirka Kolarikova                                        */
/*   Datum:          24.4.2020                                               */
/*                                                                           */
/* ==========================================================================*/

// used sources: 
// https://www.binarytides.com/packet-sniffer-code-c-libpcap-linux-sockets/
// https://www.tcpdump.org/pcap.html
// http://embeddedguruji.blogspot.com/2014/01/pcapfindalldevs-example.html
// https://stackoverflow.com/questions/28566424/linux-networking-gethostbyaddr


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <pcap/pcap.h>
#include <string.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <net/ethernet.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <time.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//prints out all interfaces present on system
//original author: http://embeddedguruji.blogspot.com/2014/01/pcapfindalldevs-example.html
void print_devices(){
  char error[PCAP_ERRBUF_SIZE];
  pcap_if_t *interfaces,*temp;
  int i=0;
  if(pcap_findalldevs(&interfaces,error)==-1){
    printf("\nerror in pcap findall devs");
    exit(12);   
  }
  printf("the interfaces present on the system are:\n");
  for(temp=interfaces;temp;temp=temp->next){
    printf("%d: '%s': %s\n",i++,temp->name, temp->description);
  }
}

// converts ip adress to host name and prints 
// author: https://stackoverflow.com/questions/28566424/linux-networking-gethostbyaddr
void get_host_name(char *address){
  char *tempstring = address;
  struct sockaddr_in sa;
  char hbuf[NI_MAXHOST];
  memset(&sa, 0, sizeof sa);
  sa.sin_family = AF_INET;
  inet_pton(AF_INET, address, &sa.sin_addr);
  if (getnameinfo((struct sockaddr*)&sa, sizeof(sa),hbuf, sizeof(hbuf),
                  NULL, 0, NI_NAMEREQD)) {
    printf("%s", tempstring);
  }else{
    printf("%s", hbuf);
  }
}


// prints packet bytes in hexa and ascii to stdout
// original author: https://www.binarytides.com/packet-sniffer-code-c-libpcap-linux-sockets/
int PrintData (const unsigned char* data , int Size, int offset)
{
  int i , j;
  for(i=0 ; i < Size ; i++){
    if( i!=0 && i%16==0){  //if one line of hex printing is complete...
      printf("   ");
      //prints bytes in ascii
      for(j=i-16 ; j<i ; j++){
        if(j%8==0 && j%16!=0) printf(" ");
        if(data[j]>=32 && data[j]<=128)
          printf("%c",(unsigned char)data[j]); //if its a number or alphabet
        else printf("."); //otherwise print a dot
      }
    printf("\n");
    } 
    //prints bytes size in hexadecimal
    if(i%16==0) printf("0x%04x: ", offset);
    //prints space between 8. and 9. byte
    else if(i%8==0) printf(" ");

    //prints bytes in hexadecimal
    printf("%02x ",(unsigned int)data[i]);
    offset++;
                
    //final line 
    if( i==Size-1){  //print the last spaces
      printf(" ");
      for(j=0;j<15-i%16;j++){
        printf("   ");} //extra spaces
             
      printf( "   ");
             
      for(j=i-i%16 ; j<=i ; j++){
        if(data[j]>=32 && data[j]<=128) {
          printf("%c",(unsigned char)data[j]);
        }else{
          printf( ".");
        }
      }
      printf("\n" );
    }
  }
  return offset;
}

//prints udp packet info
// original author: https://www.binarytides.com/packet-sniffer-code-c-libpcap-linux-sockets/
void print_udp_packet(const u_char *Buffer , int Size)
{

  struct sockaddr_in source,dest;

  unsigned short iphdrlen;
  struct iphdr *iph = (struct iphdr *)(Buffer +  sizeof(struct ethhdr));
  iphdrlen = iph->ihl*4;
  struct udphdr *udph = (struct udphdr*)(Buffer + iphdrlen  + sizeof(struct ethhdr));
  int header_size =  sizeof(struct ethhdr) + iphdrlen + sizeof udph;

  //get time
  time_t timer;
  char time_buff[26];
  struct tm* tm_info;
  timer = time(NULL);
  tm_info = localtime(&timer);
  strftime(time_buff, 26, "%H:%M:%S", tm_info);

  //print header

  //time
  printf("%s ",time_buff);

  //source IP : PORT
  unsigned temper = ntohs(udph->source);
  memset(&source, 0, sizeof(source));
  source.sin_addr.s_addr = iph->saddr;
  get_host_name(inet_ntoa(source.sin_addr));
  printf(" : %u > ", temper );

  //destination IP : PORT
  memset(&dest, 0, sizeof(dest));
  dest.sin_addr.s_addr = iph->daddr;
  get_host_name(inet_ntoa(dest.sin_addr));
  printf(" : %u\n", ntohs(udph->dest) );

  printf("\n");

  //print packet info in hex and ascii
  int offset = PrintData(Buffer , iphdrlen, 0);
  offset =  PrintData(Buffer+iphdrlen , sizeof udph, offset);
  printf("\n");
  offset = PrintData(Buffer + header_size , Size - header_size, offset);
  printf("\n");
}

//prints tcp packket info
// original author: https://www.binarytides.com/packet-sniffer-code-c-libpcap-linux-sockets/
void print_tcp_packet(const unsigned char* Buffer, int Size)
{

  struct sockaddr_in source,dest;
  unsigned short iphdrlen;
  struct iphdr *iph = (struct iphdr *)( Buffer  + sizeof(struct ethhdr) );
  iphdrlen = iph->ihl*4;
  struct tcphdr *tcph=(struct tcphdr*)(Buffer + iphdrlen + sizeof(struct ethhdr));         
  int header_size =  sizeof(struct ethhdr) + iphdrlen + tcph->doff*4;

  //get time
  time_t timer;
  char time_buff[26];
  struct tm* tm_info;
  timer = time(NULL);
  tm_info = localtime(&timer);
  strftime(time_buff, 26, "%H:%M:%S", tm_info);
  printf("%s ", time_buff);

  //source IP : PORT
  memset(&source, 0, sizeof(source));
  source.sin_addr.s_addr = iph->saddr;
  get_host_name(inet_ntoa(source.sin_addr));
  printf(" : %u > ", ntohs(tcph->source) );

  //detination IP : PORT
  memset(&dest, 0, sizeof(dest));
  dest.sin_addr.s_addr = iph->daddr;
  get_host_name(inet_ntoa(dest.sin_addr));
  printf(" : %u\n",  ntohs(tcph->dest) );
  printf("\n");

  //print packet info in hex and ascii
  int offset = PrintData(Buffer,iphdrlen, 0);
  offset = PrintData(Buffer+iphdrlen,tcph->doff*4, offset);
  printf("\n");
  offset = PrintData(Buffer + header_size , Size - header_size, offset);
  printf("\n");
}


//processes the packet and prints info
// original author: https://www.binarytides.com/packet-sniffer-code-c-libpcap-linux-sockets/
void process_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *buffer){

  int size = header->len;
  
  //Get the IP Header part of this packet
  struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));
  switch (iph->protocol) //Check the Protocol
  {
    
    case 6:  //TCP Protocol
      //printf("TCP arrived!\n");
      print_tcp_packet(buffer , size);
      break;
    
    case 17: //UDP Protocol
      //printf("UDP arrived!\n");
      print_udp_packet(buffer , size);
      break;
    
    default: //other
      break;
  }
}


//main function, parses command line arguments
//some parts taken from: https://www.tcpdump.org/pcap.html
int main(int argc, char **argv){

  char *device = (char*) "";     //string for device name -i
  char *port = (char*) "";       //string for port -p
  char *packet_num = (char*) ""; //string for -n 
  int packet_num_int = 1;        //default number of packets
  bool tcp = false;              //-t sets true
  bool udp = false;              //-u sets true

  //argument check
	int c;
	while (1){
      static struct option long_options[] ={
          {"udp",     no_argument,      0, 'u'},
          {"tcp",     no_argument,      0, 't'},
          {0, 0, 0, 0}
        };
      int option_index = 0;
      c = getopt_long (argc, argv, ":i::p::n::ut",
                       long_options, &option_index);
      //end of command line arguments
      if (c == -1)
        break;

      switch (c){
        case 0:
          break;

        case 'u':
          udp = true;
          break;

        case 't':
          tcp = true;
          break;

        case 'i':
          //saves -i value into device
          if(optarg){ device = optarg;}
          else if(!optarg
           && NULL != argv[optind]
           && '-' != argv[optind][0] ) {
            device = argv[optind++];}
          break;

        case 'p':
          //saves -p value into port
          if(optarg){ port = optarg;}
          else if(   !optarg
           && NULL != argv[optind]
           && '-' != argv[optind][0] ) {
            port = argv[optind++];}
          break;

        case 'n':
          //saves -n value into packet_num_int
          if(optarg){ packet_num = optarg;}
          else if(   !optarg
           && NULL != argv[optind]
           && '-' != argv[optind][0] ) {
            packet_num = argv[optind++];}
        	//must be an integer
        	char *znaky;
        	char packet_num_array[1024];
        	strcpy(packet_num_array, packet_num);
			   packet_num_int = strtol(packet_num_array, &znaky, 10);
			   if(strlen(znaky)>0){
				    fprintf(stderr, "-n NUM must be an integer value!\n");
				    exit(11);}
         if(packet_num_int < 0){
            fprintf(stderr, "-n NUM must be a positive integer value!\n");
            exit(11);}
          break;

        case '?':
          fprintf(stderr,"Unknown command line argument.\n");
          exit(10);
          break;

        default:
          abort ();
        }
    }
	

  //print avaiable devices and exit
  if (*device == '\0'){
    print_devices();
    exit(0);
  }
  //open device
  char errbuf[PCAP_ERRBUF_SIZE]; //error string
  pcap_t *handle; //sesion handle
  handle = pcap_open_live(device, BUFSIZ, 1, 1000, errbuf);
  if (handle == NULL) {
    fprintf(stderr, "Couldn't open device %s: %s\n", device, errbuf);
    return(2);
  }

  //set filter
  char filter[1000];
  if((udp == true && tcp == true) || (udp == false && tcp == false)){
    strcpy(filter, "(tcp or udp)");
  }
  else if(udp == true){
    strcpy(filter, "udp");
  }
  else if(tcp == true){
    strcpy(filter, "tcp");
  }

  char new_filter[1024];
  strcpy(new_filter, filter);

  //set filter for port
  if( *port != '\0'){
    char port_string[] = " and port ";

    strcat(port_string, port);

    strcat(new_filter, port_string);
  }


  //filter packages
  struct bpf_program fp; //compiled filter expression
  bpf_u_int32 mask;   // netmask of sniffing device 
  bpf_u_int32 net;   // The IP of sniffing device


  if (pcap_lookupnet(device, &net, &mask, errbuf) == -1) {
    fprintf(stderr, "Can't get netmask for device %s\n", device);
    net = 0;
    mask = 0;
  }
  if (pcap_compile(handle, &fp, new_filter, 0, net) == -1) {
    fprintf(stderr, "Couldn't parse filter %s: %s\n", new_filter, pcap_geterr(handle));
    return(2);
  }
  if (pcap_setfilter(handle, &fp) == -1) {
    fprintf(stderr, "Couldn't install filter %s: %s\n", new_filter, pcap_geterr(handle));
    return(2);
  }


  //find all n packages
  pcap_loop(handle , packet_num_int , process_packet , NULL);

	return 0;
}



