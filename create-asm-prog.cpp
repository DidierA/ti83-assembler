#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>

using namespace std;

vector<unsigned char> byte_vector;

int memorylocation;
int start_counting=0;
int program_index=0;

class label;

vector<label> label_vector;
vector<label> label_cr_vector;

class label
{
public:
  label()
  {
    index=program_index;
    memory_location=memorylocation;
    label_number=-1;
#ifdef DEBUG
    cerr << "creating label: [" << hex << memory_location << "]" << endl;
#endif
  };
  label(int l)
  {
    index=program_index;
    memory_location=memorylocation;
    label_number=l;
#ifdef DEBUG
    cerr << "creating label: [prg]" << dec << index << " links to label#" << dec << label_number << endl;
#endif
  };
  ~label(){};
  int getIndex(){ return index; };
  int getMemoryLocation(){ return memory_location; };
  int gerLabelNumber(){ return label_number; };
  void add()
  {
#ifdef DEBUG
    cerr << "changing bytes: [" << index << " and " << index+1 << "] to ["
	 << hex << (label_cr_vector[label_number-1].getMemoryLocation()&0xFF) << " and "
	 << hex << ( (label_cr_vector[label_number-1].getMemoryLocation()&0xFF00 )/0xFF) << "]" <<  endl;
#endif 
    byte_vector[index] = (label_cr_vector[label_number-1].getMemoryLocation()&0xFF);// L
    byte_vector[index+1] = ((label_cr_vector[label_number-1].getMemoryLocation()&0xFF00)/0xFF );// H
  };
private:
  int index;
  int memory_location;
  int label_number;
};


void startCounting()
{
  start_counting=1;
  memorylocation=40341;
}

void addLabel()
{
  label l = label();
  label_cr_vector.push_back( l );
}

void addLabel( int i )
{
  label l = label(i);
  label_vector.push_back( l );
}

void addByte( unsigned char b )
{
  program_index++;
  if( start_counting == 1) memorylocation++;  
  byte_vector.push_back( (unsigned char) b );
}

void addWord( int w )
{
  addByte( w&0xFF );
  addByte( (w&0xFF00 )/0xFF );
}
void addWord()
{
  for( int i=0; i<2; i++ )
    {
      program_index++;
      if( start_counting == 1) memorylocation++;  
      byte_vector.push_back( (unsigned char) 0x00 );
    }
}
void addString( string s )
{
  for( int i=0; i<s.size(); i++ )
    {
      addByte( s[i] );
    }
  if( start_counting == 1 ) addByte( 0x00 );
}

void setComment( string s )
{
  s.resize(42,0x00);
  addString(s);
  
}
void setName( string s )
{
  s.resize(8,0x00);
  cerr << "NAME: " << s << endl;
  addString(s);
}

void sysCall( string s )
{
    addByte( 0xEF ); // rst $28
    if( s == "PutS")
      {
	addWord(0x450A);
      }
    else if( s == "NewLine" )
      {
	addWord(0x452E);
      }
    else if( s == "DispHL" )
      {
	addWord(0x4507);
      }
    else if( s == "CpOP1ToOP2" )
      {
	addWord(0x4111);
      }
    else if( s == "CpOP4ToOP3" )
      {
	addWord(0x4108);
      }
    else if( s == "Mov9ToOP1" )
      {
	addWord(0x417A);
      }
    else if( s == "Mov9ToOP2" )
      {
	addWord(0x4180);
      }
    else if( s == "Mov9B" )
      {
	addWord(0x415F);
      }
    else if( s == "OP1ExOP2" )
      {
	addWord(0x421F);
      }
    else if( s == "RandInit" )
      {
	addWord(0x4B7F);
      }
    else if( s == "Random" )
      {
	addWord(0x4B79);
      }
    else if( s == "RclX" )
      {
	addWord(0x4AE0);
      }
    else if( s == "StoX" )
      {
	addWord(0x4AD1);
      }

}

void a( string s )
{
  if( s == "ret" )
    {
      addByte( 0xC9 );
    }
  else if( s == "nop" )
    {
      addByte( 0x00 );
    }
  else if( s == "ld bc, **" )
    {
      addByte(0x01);
    }
  else if( s == "ld (bc), a" )
    {
      addByte(0x02);
    }
  else if( s == "inc bc" )
    {
      addByte(0x03);
    }
  else if( s == "inc b" )
    {
      addByte(0x04);
    }
  else if( s == "dec b" )
    {
      addByte(0x05);
    }
  else if( s == "ld b, *" )
    {
      addByte(0x06);
    }
  else if( s == "rlca" )
    {
      addByte(0x07);
    }
  else if( s == "ex af, af'" )
    {
      addByte(0x08);
    }
  else if( s == "add hl, bc" )
    {
      addByte(0x09);
    }
  else if( s == "ld a, (bc)" )
    {
      addByte(0x0A);
    }
  else if( s == "dec bc" )
    {
      addByte(0x0B);
    }
  else if( s == "inc c" )
    {
      addByte(0x0C);
    }
  else if( s == "dec c" )
    {
      addByte(0x0D);
    }
  else if( s == "ld c, *" )
    {
      addByte(0x0E);
    }
  else if( s == "rrca" )
    {
      addByte(0x0F);
    }
  else if( s == "djnz *" )
    {
      addByte(0x10);
    }
  else if( s == "ld de, **" )
    {
      addByte(0x11);
    }
  else if( s == "ld (de), a" )
    {
      addByte(0x12);
    }
  else if( s == "inc de" )
    {
      addByte(0x13);
    }
  else if( s == "inc d" )
    {
      addByte(0x14);
    }
  else if( s == "dec d" )
    {
      addByte(0x15);
    }
  else if( s == "ld d, *" )
    {
      addByte(0x16);
    }
  else if( s == "rla" )
    {
      addByte(0x17);
    }
  else if( s == "jr *" )
    {
      addByte(0x18);
    }
  else if( s == "add hl, de" )
    {
      addByte(0x19);
    }
  else if( s == "ld a, (de)" )
    {
      addByte(0x1A);
    }
  else if( s == "dec de" )
    {
      addByte(0x1B);
    }
  else if( s == "inc e" )
    {
      addByte(0x1C);
    }
  else if( s == "dec e" )
    {
      addByte(0x1D);
    }
  else if( s == "ld e, *" )
    {
      addByte(0x1E);
    }
  else if( s == "rra" )
    {
      addByte(0x1F);
    }
  else if( s == "jr nz, *" )
    {
      addByte(0x20);
    }
  else if( s == "ld hl, **" )
    {
      addByte(0x21);
    }
  else if( s == "ld (**), hl" )
    {
      addByte(0x22);
    }
  else if( s == "inc hl" )
    {
      addByte(0x23);
    }
  else if( s == "inc h" )
    {
      addByte(0x24);
    }
  else if( s == "dec h" )
    {
      addByte(0x25);
    }
  else if( s == "ld h, *" )
    {
      addByte(0x26);
    }
  else if( s == "daa" )
    {
      addByte(0x27);
    }
  else if( s == "jr z, *" )
    {
      addByte(0x28);
    }
  else if( s == "add hl, hl" )
    {
      addByte(0x29);
    }
  else if( s == "ld hl, (**)" )
    {
      addByte(0x2A);
    }
  else if( s == "dec hl" )
    {
      addByte(0x2B);
    }
  else if( s == "inc l" )
    {
      addByte(0x2C);
    }
  else if( s == "dec l" )
    {
      addByte(0x2D);
    }
  else if( s == "ld l, *" )
    {
      addByte(0x2E);
    }
  else if( s == "cpl" )
    {
      addByte(0x2F);
    }
  else if( s == "jr nc, *" )
    {
      addByte(0x30);
    }
  else if( s == "ld sp, **" )
    {
      addByte(0x31);
    }
  else if( s == "ld (**), a" )
    {
      addByte(0x32);
    }
  else if( s == "inc sp" )
    {
      addByte(0x33);
    }
  else if( s == "inc (hl)" )
    {
      addByte(0x34);
    }
  else if( s == "dec (hl)" )
    {
      addByte(0x35);
    }
  else if( s == "ld (hl), *" )
    {
      addByte(0x36);
    }
  else if( s == "scf" )
    {
      addByte(0x37);
    }
  else if( s == "jr c, *" )
    {
      addByte(0x38);
    }
  else if( s == "add hl, sp" )
    {
      addByte(0x39);
    }
  else if( s == "ld a, (**)" )
    {
      addByte(0x3A);
    }
  else if( s == "dec sp" )
    {
      addByte(0x3B);
    }
  else if( s == "inc a" )
    {
      addByte(0x3C);
    }
  else if( s == "dec a" )
    {
      addByte(0x3D);
    }
  else if( s == "ld a, *" )
    {
      addByte(0x3E);
    }
  else if( s == "ccf" )
    {
      addByte(0x3F);
    }
  else if( s == "ld b, b" )
    {
      addByte(0x40);
    }
  else if( s == "ld b, c" )
    {
      addByte(0x41);
    }
  else if( s == "ld b, d" )
    {
      addByte(0x42);
    }
  else if( s == "ld b, e" )
    {
      addByte(0x43);
    }
  else if( s == "ld b, h" )
    {
      addByte(0x44);
    }
  else if( s == "ld b, l" )
    {
      addByte(0x45);
    }
  else if( s == "ld b, (hl)" )
    {
      addByte(0x46);
    }
  else if( s == "ld b, a" )
    {
      addByte(0x47);
    }
  else if( s == "ld c, b" )
    {
      addByte(0x48);
    }
  else if( s == "ld c, c" )
    {
      addByte(0x49);
    }
  else if( s == "ld c, d" )
    {
      addByte(0x4A);
    }
  else if( s == "ld c, e" )
    {
      addByte(0x4B);
    }
  else if( s == "ld c, h" )
    {
      addByte(0x4C);
    }
  else if( s == "ld c, l" )
    {
      addByte(0x4D);
    }
  else if( s == "ld c, (hl)" )
    {
      addByte(0x4E);
    }
  else if( s == "ld c, a" )
    {
      addByte(0x4F);
    }
  else if( s == "ld d, b" )
    {
      addByte(0x50);
    }
  else if( s == "ld d, c" )
    {
      addByte(0x51);
    }
  else if( s == "ld d, d" )
    {
      addByte(0x52);
    }
  else if( s == "ld d, e" )
    {
      addByte(0x53);
    }
  else if( s == "ld d, h" )
    {
      addByte(0x54);
    }
  else if( s == "ld d, l" )
    {
      addByte(0x55);
    }
  else if( s == "ld d, (hl)" )
    {
      addByte(0x56);
    }
  else if( s == "ld d, a" )
    {
      addByte(0x57);
    }
  else if( s == "ld e, b" )
    {
      addByte(0x58);
    }
  else if( s == "ld e, c" )
    {
      addByte(0x59);
    }
  else if( s == "ld e, d" )
    {
      addByte(0x5A);
    }
  else if( s == "ld e, e" )
    {
      addByte(0x5B);
    }
  else if( s == "ld e, h" )
    {
      addByte(0x5C);
    }
  else if( s == "ld e, l" )
    {
      addByte(0x5D);
    }
  else if( s == "ld e, (hl)" )
    {
      addByte(0x5E);
    }
  else if( s == "ld e, a" )
    {
      addByte(0x5F);
    }
  else if( s == "ld h, b" )
    {
      addByte(0x60);
    }
  else if( s == "ld h, c" )
    {
      addByte(0x61);
    }
  else if( s == "ld h, d" )
    {
      addByte(0x62);
    }
  else if( s == "ld h, e" )
    {
      addByte(0x63);
    }
  else if( s == "ld h, h" )
    {
      addByte(0x64);
    }
  else if( s == "ld h, l" )
    {
      addByte(0x65);
    }
  else if( s == "ld h, (hl)" )
    {
      addByte(0x66);
    }
  else if( s == "ld h, a" )
    {
      addByte(0x67);
    }
  else if( s == "ld l, b" )
    {
      addByte(0x68);
    }
  else if( s == "ld l, c" )
    {
      addByte(0x69);
    }
  else if( s == "ld l, d" )
    {
      addByte(0x6A);
    }
  else if( s == "ld l, e" )
    {
      addByte(0x6B);
    }
  else if( s == "ld l, h" )
    {
      addByte(0x6C);
    }
  else if( s == "ld l, l" )
    {
      addByte(0x6D);
    }
  else if( s == "ld l, (hl)" )
    {
      addByte(0x6E);
    }
  else if( s == "ld l, a" )
    {
      addByte(0x6F);
    }
  else if( s == "ld (hl), b" )
    {
      addByte(0x70);
    }
  else if( s == "ld (hl), c" )
    {
      addByte(0x71);
    }
  else if( s == "ld (hl), d" )
    {
      addByte(0x72);
    }
  else if( s == "ld (hl), e" )
    {
      addByte(0x73);
    }
  else if( s == "ld (hl), h" )
    {
      addByte(0x74);
    }
  else if( s == "ld (hl), l" )
    {
      addByte(0x75);
    }
  else if( s == "halt" )
    {
      addByte(0x76);
    }
  else if( s == "ld (hl), a" )
    {
      addByte(0x77);
    }
  else if( s == "ld a, b" )
    {
      addByte(0x78);
    }
  else if( s == "ld a, c" )
    {
      addByte(0x79);
    }
  else if( s == "ld a, d" )
    {
      addByte(0x7A);
    }
  else if( s == "ld a, e" )
    {
      addByte(0x7B);
    }
  else if( s == "ld a, h" )
    {
      addByte(0x7C);
    }
  else if( s == "ld a, l" )
    {
      addByte(0x7D);
    }
  else if( s == "ld a, (hl)" )
    {
      addByte(0x7E);
    }
  else if( s == "ld a, a" )
    {
      addByte(0x7F);
    }
  else if( s == "add a,b" ){addByte(0x80);}
  else if( s == "add a,c" ){addByte(0x81);}
  else if( s == "add a,d" ){addByte(0x82);}
  else if( s == "add a,e" ){addByte(0x83);}
  else if( s == "add a,h" ){addByte(0x84);}
  else if( s == "add a,l" ){addByte(0x85);}
  else if( s == "add a,(hl)" ){addByte(0x86);}
  else if( s == "add a,a" ){addByte(0x87);}
  else if( s == "adc a,b" ){addByte(0x88);}
  else if( s == "adc a,c" ){addByte(0x89);}
  else if( s == "adc a,d" ){addByte(0x8A);}
  else if( s == "adc a,e" ){addByte(0x8B);}
  else if( s == "adc a,h" ){addByte(0x8C);}
  else if( s == "adc a,l" ){addByte(0x8D);}
  else if( s == "adc a,(hl)" ){addByte(0x8E);}
  else if( s == "adc a,a" ){addByte(0x8F);}
  else if( s == "subb" ){addByte(0x90);}
  else if( s == "subc" ){addByte(0x91);}
  else if( s == "subd" ){addByte(0x92);}
  else if( s == "sube" ){addByte(0x93);}
  else if( s == "subh" ){addByte(0x94);}
  else if( s == "subl" ){addByte(0x95);}
  else if( s == "sub(hl)" ){addByte(0x96);}
  else if( s == "suba" ){addByte(0x97);}
  else if( s == "sbc a,b" ){addByte(0x98);}
  else if( s == "sbc a,c" ){addByte(0x99);}
  else if( s == "sbc a,d" ){addByte(0x9A);}
  else if( s == "sbc a,e" ){addByte(0x9B);}
  else if( s == "sbc a,h" ){addByte(0x9C);}
  else if( s == "sbc a,l" ){addByte(0x9D);}
  else if( s == "sbc a,(hl)" ){addByte(0x9E);}
  else if( s == "sbc a,a" ){addByte(0x9F);}
  else if( s == "add a, b" ){addByte(0x80);}
  else if( s == "add a, c" ){addByte(0x81);}
  else if( s == "add a, d" ){addByte(0x82);}
  else if( s == "add a, e" ){addByte(0x83);}
  else if( s == "add a, h" ){addByte(0x84);}
  else if( s == "add a, l" ){addByte(0x85);}
  else if( s == "add a, (hl)" ){addByte(0x86);}
  else if( s == "add a, a" ){addByte(0x87);}
  else if( s == "adc a, b" ){addByte(0x88);}
  else if( s == "adc a, c" ){addByte(0x89);}
  else if( s == "adc a, d" ){addByte(0x8A);}
  else if( s == "adc a, e" ){addByte(0x8B);}
  else if( s == "adc a, h" ){addByte(0x8C);}
  else if( s == "adc a, l" ){addByte(0x8D);}
  else if( s == "adc a, (hl)" ){addByte(0x8E);}
  else if( s == "adc a, a" ){addByte(0x8F);}
  else if( s == "sub b" ){addByte(0x90);}
  else if( s == "sub c" ){addByte(0x91);}
  else if( s == "sub d" ){addByte(0x92);}
  else if( s == "sub e" ){addByte(0x93);}
  else if( s == "sub h" ){addByte(0x94);}
  else if( s == "sub l" ){addByte(0x95);}
  else if( s == "sub (hl)" ){addByte(0x96);}
  else if( s == "sub a" ){addByte(0x97);}
  else if( s == "sbc a, b" ){addByte(0x98);}
  else if( s == "sbc a, c" ){addByte(0x99);}
  else if( s == "sbc a, d" ){addByte(0x9A);}
  else if( s == "sbc a, e" ){addByte(0x9B);}
  else if( s == "sbc a, h" ){addByte(0x9C);}
  else if( s == "sbc a, l" ){addByte(0x9D);}
  else if( s == "sbc a, (hl)" ){addByte(0x9E);}
  else if( s == "sbc a, a" ){addByte(0x9F);}
  else if( s == "and b" ){addByte(0xA0);}
  else if( s == "and c" ){addByte(0xA1);}
  else if( s == "and d" ){addByte(0xA2);}
  else if( s == "and e" ){addByte(0xA3);}
  else if( s == "and h" ){addByte(0xA4);}
  else if( s == "and l" ){addByte(0xA5);}
  else if( s == "and (hl)" ){addByte(0xA6);}
  else if( s == "and a" ){addByte(0xA7);}
  else if( s == "and b" ){addByte(0xA8);}
  else if( s == "xor c" ){addByte(0xA9);}
  else if( s == "xor d" ){addByte(0xAA);}
  else if( s == "xor e" ){addByte(0xAB);}
  else if( s == "xor h" ){addByte(0xAC);}
  else if( s == "xor l" ){addByte(0xAD);}
  else if( s == "xor (hl)" ){addByte(0xAE);}
  else if( s == "xor a" ){addByte(0xAF);}
  else if( s == "or b" ){addByte(0xB0);}
  else if( s == "or c" ){addByte(0xB1);}
  else if( s == "or d" ){addByte(0xB2);}
  else if( s == "or e" ){addByte(0xB3);}
  else if( s == "or h" ){addByte(0xB4);}
  else if( s == "or l" ){addByte(0xB5);}
  else if( s == "or (hl)" ){addByte(0xB6);}
  else if( s == "or a" ){addByte(0xB7);}
  else if( s == "cp b" ){addByte(0xB8);}
  else if( s == "cp c" ){addByte(0xB9);}
  else if( s == "cp d" ){addByte(0xBA);}
  else if( s == "cp e" ){addByte(0xBB);}
  else if( s == "cp h" ){addByte(0xBC);}
  else if( s == "cp l" ){addByte(0xBD);}
  else if( s == "cp (hl)" ){addByte(0xBE);}
  else if( s == "cp a" ){addByte(0xBF);}
  else if( s == "ret nz" ){addByte(0xC0);}
  else if( s == "pop bc" ){addByte(0xC1);}
  else if( s == "jp nz, **" ){addByte(0xC2);}
  else if( s == "jp **" ){addByte(0xC3);}
  else if( s == "call nz, **" ){addByte(0xC4);}
  else if( s == "push bc" ){addByte(0xC5);}
  else if( s == "add a, *" ){addByte(0xC6);}
  else if( s == "rst 0x00" ){addByte(0xC7);}
  else if( s == "ret z" ){addByte(0xC8);}
  else if( s == "ret" ){addByte(0xC9);}
  else if( s == "jp z, **" ){addByte(0xCA);}
  else if( s == "BITS" )
    {
      cerr << "BITS Not Yet Implemented" << endl;
      addByte(0xCB);
    }
  else if( s == "call z, **" ){addByte(0xCC);}
  else if( s == "call **" ){addByte(0xCD);}
  else if( s == "adc a, *" ){addByte(0xCE);}
  else if( s == "rst 0x08" ){addByte(0xCF);}
  else if( s == "ret nc" ){addByte(0xD0);}
  else if( s == "pop de" ){addByte(0xD1);}
  else if( s == "jp nc, **" ){addByte(0xD2);}
  else if( s == "out (*), a" ){addByte(0xD3);}
  else if( s == "call nc, **" ){addByte(0xD4);}
  else if( s == "push de" ){addByte(0xD5);}
  else if( s == "sub *" ){addByte(0xD6);}
  else if( s == "rst 0x10" ){addByte(0xD7);}
  else if( s == "ret c" ){addByte(0xD8);}
  else if( s == "exx" ){addByte(0xD9);}
  else if( s == "jp c, **" ){addByte(0xDA);}
  else if( s == "in a, (*)" ){addByte(0xDB);}
  else if( s == "call c, **" ){addByte(0xDC);}
  else if( s == "IX" )
    {
      cerr << "IX Not Yet Implemented" << endl;
      addByte(0xDD);
    }
  else if( s == "sbc a, *" ){addByte(0xDE);}
  else if( s == "rst 0x18" ){addByte(0xDF);}
  else if( s == "ret po" ){addByte(0xE0);}
  else if( s == "pop hl" ){addByte(0xE1);}
  else if( s == "jp po, **" ){addByte(0xE2);}
  else if( s == "ex (sp), hl" ){addByte(0xE3);}
  else if( s == "call po, **" ){addByte(0xE4);}
  else if( s == "push hl" ){addByte(0xE5);}
  else if( s == "and *" ){addByte(0xE6);}
  else if( s == "rst 0x20" ){addByte(0xE7);}
  else if( s == "ret pe" ){addByte(0xE8);}
  else if( s == "jp (hl)" ){addByte(0xE9);}
  else if( s == "jp pe, **" ){addByte(0xEA);}
  else if( s == "ex de, hl" ){addByte(0xEB);}
  else if( s == "call pe, **" ){addByte(0xEC);}
  else if( s == "EXTD" )
    {
      cerr << "EXTD Not Yet Implemented" << endl;
      addByte(0xED);
    }
  else if( s == "xor *" ){addByte(0xEE);}
  else if( s == "rst 0x28" ){addByte(0xEF);}
  else if( s == "ret p" ){addByte(0xF0);}
  else if( s == "pop af" ){addByte(0xF1);}
  else if( s == "jp p, **" ){addByte(0xF2);}
  else if( s == "di" ){addByte(0xF3);}
  else if( s == "call p, **" ){addByte(0xF4);}
  else if( s == "push af" ){addByte(0xF5);}
  else if( s == "or *" ){addByte(0xF6);}
  else if( s == "rst 0x30" ){addByte(0xF7);}
  else if( s == "ret m" ){addByte(0xF8);}
  else if( s == "ld sp, hl" ){addByte(0xF9);}
  else if( s == "jp m, **" ){addByte(0xFA);}
  else if( s == "ei" ){addByte(0xFB);}
  else if( s == "call m, **" ){addByte(0xFC);}
  else if( s == "IY" )
    {
      cerr << "IY Not Yet Implemented" << endl;
      addByte(0xFD);
    }
  else if( s == "cp *" ){addByte(0xFE);}
  else if( s == "rst 0x38" ){addByte(0xFF);}

  //else if( s == "" )
  //  {
  //    addByte(0x);
  //  }

}
int main(int argc, char *argv[])
{
  if( argc != 2 )
    {
      cerr << "you must supply an output filename" << endl;
      exit(-1);
    }
  FILE *binary = fopen(argv[1], "wb");

  addString( "**TI83F*" );
  addByte( 0x1A );
  addByte( 0x0A );
  addByte( 0x00 );
  setComment( "http://mkpelleg.freeshell.org" );
  addByte( 0xFF ); // PROGRAM LENGTH - 57 (L)  (53) (This will get overwritten)
  addByte( 0xFF ); // PROGRAM LENGTH - 57 (H)  (54) (This will get overwritten)
  addByte( 0x0D ); 
  addByte( 0x00 );

  // PROGRAM START
  addByte( 0xFF ); // len of Machine Code + 2 bytes (L) (This will get overwritten)
  addByte( 0xFF ); // len of Machine Code + 2 bytes (H) (This will get overwritten)
  addByte( 0x06 );
  setName( "TEST1" ); // MUST BE 8 BYTES!!!
  addByte( 0x00 );
  addByte( 0x00 );
  addByte( 0xFF ); // len of Machine Code + 2 bytes (L)  (This will get overwritten)
  addByte( 0xFF ); // len of Machine Code + 2 bytes (H) (This will get overwritten)
  addByte( 0xFF ); // Machine Code Size (L)  (This will get overwritten)
  addByte( 0xFF ); // Machine Code Size (H) (This will get overwritten)

  addByte( 0xBB ); // Machine Code
  addByte( 0x6D );
  startCounting();
  // ==================================================
  a( "ld hl, **" ); addLabel(1); addWord();
  sysCall("PutS");
  
  sysCall("NewLine");

  a( "ld hl, **"); addLabel(2); addWord();
  sysCall("PutS");

  sysCall("NewLine");


  a( "ld hl, **"); addWord( 0x1234 );
  sysCall( "DispHL" );
  
  a( "ret" );

  // $9D9F
  addLabel(); addString( "Written in Asm" );
  addLabel(); addString( "x:" );

  // ==================================================
  // redo labels
  for( int i=0; i<label_vector.size(); i++ )
    {
      label_vector[i].add();
    }
  
  // Set Program Size-57
  int psH,psL=0;
  psH=((byte_vector.size()-55) & 0xFF00)/0xFF; // it's -55 (instead of -57) because we haven't added in the Checksum yet
  psL=((byte_vector.size()-55) & 0xFF); // it's -55 (instead of -57) because we haven't added in the Checksum yet
  byte_vector[53]=psL;
  byte_vector[54]=psH;

  // Set Machine Code Sizes -74
  int mcsH,mcsL=0;
  mcsH=((byte_vector.size()-72) & 0xFF00)/0xFF; // it's -72 (instead of -74) because we haven't added in the Checksum yet
  mcsL=((byte_vector.size()-72) & 0xFF); // it's -72 (instead of -74) because we haven't added in the Checksum yet
  byte_vector[57]=mcsL;
  byte_vector[58]=mcsH;
  byte_vector[70]=mcsL;
  byte_vector[71]=mcsH;
  // Set Just the Code Size
  mcsH=((byte_vector.size()-74) & 0xFF00)/0xFF;
  mcsL=((byte_vector.size()-74) & 0xFF);
  byte_vector[72]=mcsL;
  byte_vector[73]=mcsH;
  
  // calculate the checksum
  int cs=0;
  for( int i=55; i<byte_vector.size(); i++ )
    {
      cs+=byte_vector[i];
    }
  addByte( cs&0xFF );
  addByte( (cs & 0xFF00)/0xFF );

  for( int i=0; i<byte_vector.size(); i++ )
    {
      unsigned char byte_to_write = byte_vector[i];
      fwrite(&byte_to_write, 1, sizeof(byte_to_write), binary);
    }
  return 0;
}
