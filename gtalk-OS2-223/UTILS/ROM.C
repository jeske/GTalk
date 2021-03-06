
#include <time.h>
#include <stdio.h>

#define XOR_FACTOR_1 0x45AF3214l
#define XOR_FACTOR_2 0xA5B2F321l
#define XOR_FACTOR_3 0xF74937ABl

unsigned short int rom_checksum(void)
{
  unsigned char *rom_pass = (unsigned char *) 0xF0000000;
  unsigned short int checksum = 0;

  while (rom_pass<(unsigned char *)0xF0000080) checksum += *rom_pass++;
  return (checksum);
}

unsigned long int hex_conversion(const char *str)
{
	unsigned long int temp = 0;
	unsigned char digit;
	while (*str)
	{
		if ((*str>='0') && (*str<='f'))
		{
		 digit = *str++ - '0';
		 if (digit > 48) digit -= ' ';
		 if (digit > 0x09) digit -= 0x07;
		 temp = (temp << 4) | (unsigned long int) (digit & 0x0F);
		} else str++;
	}
	return (temp);
}

unsigned long int scramble(unsigned int composite,
 unsigned char nodes, unsigned char system_no)
{
  unsigned short int second_int = (((unsigned int) nodes) << 8) | system_no;
  unsigned long int temp = 0;
  int bits;

  for (bits=0;bits<16;bits++)
  {
	temp = (temp << 1) | (second_int >> 15);
	second_int <<= 1;
	temp = (temp << 1) | (composite >> 15);
	composite <<= 1;
  }
  temp ^= XOR_FACTOR_1;
  return (temp);
}

void un_bit_shuffle(unsigned long int shuffled_bit,
	unsigned short int *first_number, unsigned short int *second_number)
{
  int bits;

  shuffled_bit ^= XOR_FACTOR_3;

  for (bits=0;bits<16;bits++)
  {
	*second_number = (*second_number << 1) | (shuffled_bit >> 31);
	shuffled_bit <<= 1;
	*first_number  = (*first_number << 1) | (shuffled_bit >> 31);
	shuffled_bit <<= 1;
  }
}

void unscramble(unsigned long int big_checksum, unsigned short int *composite,
  unsigned char *nodes, unsigned char *system_no)
{
  unsigned short int second_int = 0;
  int bits;

  big_checksum ^= XOR_FACTOR_1;
  *composite = 1;
  for (bits=0;bits<16;bits++)
  {
	second_int = (second_int << 1) | (big_checksum >> 31);
	big_checksum <<= 1;
	*composite = (*composite << 1) | (big_checksum >> 31);
	big_checksum <<= 1;
  }
  *nodes = second_int >> 8;
  *system_no = (unsigned char) second_int;
}

void main(void)
{
  char s[80];
  unsigned short int rom_checksum;
  unsigned short int file_checksum;
  unsigned short int composite;
  unsigned long int long_composite;
  unsigned char nodes;
  unsigned char system_no;
  unsigned long int checksum;
  unsigned long int verify;
  unsigned short int calc_composite;
  unsigned short int version_no;
  time_t ourtime = time(NULL);

  printf("Enter separate checksums?");
  gets(s);
  if ((*s=='Y') || (*s=='y'))
  {
	printf("Rom checksum: ");
	gets(s);
	rom_checksum = (unsigned short int) hex_conversion(s);
	printf("File checksum: ");
	gets(s);
	file_checksum = (unsigned short int) hex_conversion(s);
	composite = rom_checksum ^ file_checksum;
  }
  else
  {
	printf("Composite (HEX) : ");
	gets(s);
	long_composite = (unsigned long int) hex_conversion(s);
	composite = (unsigned short int) long_composite;
	version_no = ((long_composite >> 16) ^ 0xAB43 ^ composite) & 0xFF;
	printf("Version No: %d\n",version_no);
  }
  printf("Vertification No : ");
  gets(s);
  verify = (unsigned long int) hex_conversion(s);
  un_bit_shuffle(verify,&rom_checksum,&file_checksum);
  calc_composite = rom_checksum ^ file_checksum;

  printf("Nodes (HEX) : ");
  gets(s);
  nodes = (unsigned char) hex_conversion(s);
  printf("System No (HEX) : ");
  gets(s);

  system_no = (unsigned char) hex_conversion(s);
  checksum = scramble(composite,nodes,system_no);
  printf("Checksum: %08lX Time: %08lX\n",checksum,
	(unsigned long int)ourtime ^ XOR_FACTOR_2);
  unscramble(checksum,&composite,&nodes,&system_no);
  printf("Composite: %04X, Nodes: %02X, System No: %02X\n",composite,nodes,system_no);
  printf("Rom checksum: %04X   File Checksum: %04X\n",rom_checksum,file_checksum);
  printf("Calculated composite checksum: %04X\n",calc_composite);
  if (calc_composite!=composite)
   {printf("ALERT! The Checksum and Verification numbers did not come\n");
   printf("       from the same location\n");
   }
}
