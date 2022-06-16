bool myHeader(char* memblock, int index){
       return (int(memblock[index * 16] & 0xFF) == 0x4e);
}
 
bool myNeutron(char* memblock, int index){
       return (int(memblock[index * 16] & 0xFF) == 0x47);
}
 
void decodeHeader(char* memblock, int index, int *data){
       int t0id = ((memblock[index * 16 + 13] & 0xFF) << 16) + ((memblock[index * 16 + 14] & 0xFF) << 8) + ((memblock[index * 16 + 15] & 0xFF));
       int ec = ((memblock[index * 16 + 9] & 0xFF) << 24) + ((memblock[index * 16 + 10] & 0xFF) << 16) + ((memblock[index * 16 + 11] & 0xFF) << 8) + ((memblock[index * 16 + 12] & 0xFF));
       int tl = ((memblock[index * 16 + 5] & 0xFF) << 16) + ((memblock[index * 16 + 6] & 0xFF) << 8) + ((memblock[index * 16 + 7] & 0xFF));
       int el = ((memblock[index * 16 + 2] & 0xF) << 16) + ((memblock[index * 16 + 3] & 0xFF) << 8) + ((memblock[index * 16 + 4] & 0xFF));
       int fl = ((memblock[index * 16 + 1] & 0xFF) << 4) + ((memblock[index * 16 + 2] & 0xF0) >> 4);
       data[0] = t0id;
       data[1] = ec;
       data[2] = tl;
       data[3] = el;
       data[4] = fl;
}
 
void decodeNeutron(char* memblock, int index, int *data){
       /*t0id*/data[0] = ((memblock[index * 16 + 13] & 0xFF) << 16) + ((memblock[index * 16 + 14] & 0xFF) << 8) + ((memblock[index * 16 + 15] & 0xFF));
       /*sx  */data[1] = ((memblock[index * 16 + 11] & 0x3) << 8) + ((memblock[index * 16 + 12] & 0xFF));
       /*dy  */data[2] = ((memblock[index * 16 + 11] & 0xFC) >> 2);
       /*sx  */data[3] = ((memblock[index * 16 + 9] & 0x3) << 8) + ((memblock[index * 16 + 10] & 0xFF));
       /*dx  */data[4] = ((memblock[index * 16 + 9] & 0xFC) >> 2);
       /*ly  */data[5] = ((memblock[index * 16 + 7] & 0x7F));
       /*fy  */data[6] = ((memblock[index * 16 + 6] & 0x3F) << 1) + ((memblock[index * 16 + 7] & 0x80) >> 7);
       /*lx  */data[7] = ((memblock[index * 16 + 5] & 0x1F) << 2) + ((memblock[index * 16 + 6] & 0xC0) >> 6);
       /*fx  */data[8] = ((memblock[index * 16 + 4] & 0xF) << 3) + ((memblock[index * 16 + 5] & 0xE0) >> 5);
       /*tof */data[9] = ((memblock[index * 16 + 1] & 0xFF) << 20) + ((memblock[index * 16 + 2] & 0xFF) << 12) + ((memblock[index * 16 + 3] & 0xFF) << 4) + ((memblock[index * 16 + 4] & 0xF0) >> 4);
}
 