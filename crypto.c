#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
//base 64 decryption start
static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decoding_table = NULL;

void build_decoding_table() {

    decoding_table = malloc(256);

    for (int i = 0; i < 64; i++)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}

unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length) {

    if (decoding_table == NULL) build_decoding_table();

    if (input_length % 4 != 0) return NULL;

    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=') (*output_length)--;
    if (data[input_length - 2] == '=') (*output_length)--;

    unsigned char *decoded_data = malloc(*output_length);
    if (decoded_data == NULL) return NULL;

    for (int i = 0, j = 0; i < input_length;) {

        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];

        uint32_t triple = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);

        if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }

    return decoded_data;
}
//base 64 decryption end

//rot 13 implementation
unsigned char *rot13_decode(const char * in, size_t n)
{
   int i;
   unsigned char *out;

    for(i=0;i<=n;i++)//updates out  with shift of 13 
	{
	    if((in[i]>='a') && (in[i]<='m'))
	        {
	            out[i]=in[i]+13;

	        }
	    else if ((in[i]>='n') && (in[i]<='z'))
	        {
	            out[i]=in[i]-13;
	        }
	}

    return out;
}

//ceasar cipher
 char * ceasar_decode(const char *message)
{
    int i,key =20;
    char  ch;
    char *out;
    for(i = 0; message[i] != '\0'; ++i){
		ch = message[i];

		if(ch >= 'a' && ch <= 'z')
		{
			ch = ch - key;//shift ch by key towards left

			if(ch < 'a')
			{
				ch = ch + 'z' - 'a' + 1;//if shifted ch is less than 'a' add 26 to bring it back to aplhabet range in ascii
			}

			out[i] = ch;
		}
		
		
	}
	return out;
}


//vingere cipher
 unsigned char * vingere_decode(const char * msg)
{

    char key[20]={"cryptography"};
    int msgLen = strlen(msg), keyLen = strlen(key), i, j;

    char newKey[msgLen];
    unsigned char * decryptedMsg= malloc(100);

    //generating new key
    for(i = 0, j = 0; i < msgLen; ++i, ++j){
        if(j == keyLen)
            j = 0;

        newKey[i] = key[j];//generating key samesize as msgLen by reccursively adding the original key
    }

    newKey[i] = '\0';



    //decryption
    for(i = 0; i < msgLen; i++)
        decryptedMsg[i] = (((msg[i] - newKey[i]) + 26) % 26) + 'a';//generate the shift from key and add it to 'a'

    decryptedMsg[i] = '\0';
    return decryptedMsg;
}



// play fair decryption


void search(char key[5][5], char a,
            char b, int arr[])
{
    int i, j;

    for (i = 0; i < 5; i++)//assigning the row no and col no of bigram to array a
    {
        for (j = 0; j < 5; j++) 
        {
            if (key[i][j] == a) 
            {
                arr[0] = i;
                arr[1] = j;
            }
            else if (key[i][j] == b) {
                arr[2] = i;
                arr[3] = j;
            }
        }
    }
}
int mod5(int a)//returns modulus of a
{   int n=a%5;
    if (n==-1)
        n=4;
    return n;
}


char * playfair_decode(  char *in)
{
	char key[5][5] ={"natds","zgrhq","ebvpm","xilfy","wcuko"};//hardcoded key
	int n= strlen(in),i,a[4];
	unsigned char * out = malloc(200);

	for(i = 0; i < n; i += 2)
       	{
       		 search(key, in[i], in[i + 1], a);
        	if (a[0] == a[2])// if the elements are in same column
	       	{
            		out[i] = key[a[0]][mod5(a[1]-1)];
            		out[i + 1] = key[a[0]][mod5(a[3]-1)];
       		 }

        	else if (a[1] == a[3])// if the elements are in same row
	       	{

           		out[i] = key[mod5(a[0] - 1)][a[1]];
           		out[i + 1] = key[mod5(a[2] - 1)][a[1]];
        	}
        	else // elements forming a rectangle
		    {
            		out[i] = key[a[0]][a[3]];
            		out[i + 1] = key[a[2]][a[1]];
        	}

    	}

	return out;
}


int main(){
    char  encoded_data[100] ;
    printf("Enter thr string to be decoded: \n");
    gets(encoded_data);
    printf("\n\n");
    printf("Encoded Data is: %s \n\n",encoded_data);
    long decode_size = strlen(encoded_data);
    char * decoded_e5 = base64_decode(encoded_data, decode_size, &decode_size);
    printf("Decoded Data of e5 is :%s \n\n",decoded_e5);
    decode_size=strlen(decoded_e5);
    char * decoded_e4 =rot13_decode(decoded_e5,decode_size);
    printf("Decoded data of e4 is :%s \n\n",decoded_e4);
    char * decoded_e3 = ceasar_decode(decoded_e4 );
    printf("Decoded Data of e3 is :%s \n\n" ,decoded_e3);
    char * decoded_e2 = vingere_decode(decoded_e3 );
    printf("Decoded Data of e2 is :%s \n\n" ,decoded_e2);
    char * decoded_e1=playfair_decode(decoded_e2);
    printf("Decoded Data of e1 is :%s",decoded_e1);
    exit(0);
}