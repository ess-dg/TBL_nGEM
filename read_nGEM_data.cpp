// adaptation from the MANTID code

#include "read_nGEM_data.h"
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <iostream>


using namespace std;

/**
 * @brief Byte swap a 64 bit word as the nGEM detector outputs in big endian
 * format. So must be swapped to have correct values on x86 and x64
 * architectures.
 *
 * @param word The 64 bit word to swap.
 * @return uint64_t The swapped word.
 */
uint64_t swapUint64(uint64_t word) {
  word = ((word << 8) & 0xFF00FF00FF00FF00ULL) | ((word >> 8) & 0x00FF00FF00FF00FFULL);
  word = ((word << 16) & 0xFFFF0000FFFF0000ULL) | ((word >> 16) & 0x0000FFFF0000FFFFULL);
  return (word << 32) | (word >> 32);
}

/**
 * @brief Correct a big endian event to be compatible with x64 and x86
 * architecture.
 *
 * @param bigEndian The big endian formatted event.
 * @param smallEndian The resulting small endian formatted event.
 */
void correctForBigEndian(EventUnion &bigEndian, EventUnion &smallEndian) {
  smallEndian.splitWord.words[0] = swapUint64(bigEndian.splitWord.words[1]);
  smallEndian.splitWord.words[1] = swapUint64(bigEndian.splitWord.words[0]);
}

int verifyFileSize(std::string filename) // path to file
{
    FILE *p_file = NULL;
    p_file = fopen(filename.c_str(),"rb");
    fseek(p_file,0,SEEK_END);
    int size = ftell(p_file);
    if (size % 16 != 0) {
    
    		cout<<"Invalid file size. File is size is " << size
                    << " bytes which is not a multiple of 16. There may be some bytes "
                       "missing from the data. \n";
                       
                       }
    fclose(p_file);
    
    return size;
}

int main(int argc,char** argv)
{


//  remove("data_for_root.txt");
  
  if (remove("data_for_root.txt") !=0 )
     puts ("File not found, continue....");
  else 
    puts("Output file data_for_root.txt succesfully deleted at the begining of the run, continue....");

	FILE* fp;
//	FILE* ofile;
    ofstream ofile;

  if (argc != 2) {
    printf("usage: read_bin filename\n");
    return -1;
  }
	const char* fn = argv[1];

	fp = fopen(fn,"rb");
//	ofile = fopen("data_for_root.txt","a");
	ofile.open("data_for_root.txt", ios::out | ios::app);
	
	if (fp==NULL)
	{
		printf("ERROR: Can't open file %s\n",fn);
		return 0;
	}


  const size_t totalNumEvents = verifyFileSize(fn) / 16;
  constexpr size_t SKIP_WORD_SIZE = 4;
  size_t numProcessedEvents = 0;
  size_t numWordsSkipped = 0;
  
  cout<<" Total number of events = "<<totalNumEvents<<endl;

  while (true) {
    // Load an event into the variable.
    // Occasionally we may get a file where the first event has been chopped,
    // so we seek to the start of a valid event.
    // Chopping only seems to occur on a 4 byte word, hence seekg() of 4
    //EventUnion defined in the header file
    
    EventUnion event, eventBigEndian;
    
    do {
      fread(reinterpret_cast<char *>(&eventBigEndian), sizeof(eventBigEndian), 1 ,fp);
      // Correct for the big endian format of nGEM datafile.
      correctForBigEndian(eventBigEndian, event);
    } while (!event.generic.check() && !fseek(fp,SKIP_WORD_SIZE, SEEK_END) && ++numWordsSkipped);
    
    if (feof(fp)) {
      break; // we have either not read an event, or only read part of one
    }
    
    if (event.coincidence.check()) { // Check for coincidence event.

      uint64_t pixelX = event.coincidence.avgX();
      uint64_t pixelY = event.coincidence.avgY();
      uint64_t X0 = event.coincidence.firstX;
      uint64_t X1 = event.coincidence.lastX;
      uint64_t Y0 = event.coincidence.firstY;
      uint64_t Y1 = event.coincidence.lastY;
      uint64_t clusTY = 5 * event.coincidence.clusterTimeY;  // because the data is 5 ns units
      uint64_t clusTX = 5 * event.coincidence.clusterTimeX;  // because the data is 5 ns units
      
      // Convert to microseconds (us)
//      const double tof = event.coincidence.timeOfFlight / 1000.0;
//      const double t0 = event.coincidence.t0id / 1000.0;
      uint64_t tof = event.coincidence.timeOfFlight;
      uint64_t t0 = event.coincidence.t0id;
              
//        if (ofile!=NULL){
				
//			fprintf(ofile,"%6.4f   0x%016llX \n",tof,pixel);
//			fprintf(ofile,"%6.4f  %6.4f   %llu  %llu  %llu  %llu   %llu   %llu   %llu\n",t0,tof,clusTX,clusTY,X0,X1,Y0,Y1,pixel);
//			fprintf(ofile,"%llu  %llu   %llu  %llu  %llu  %llu   %llu   %llu   %llu\n",t0,tof,clusTX,clusTY,X0,X1,Y0,Y1,pixel);
			ofile<<t0<<"\t"<<tof<<"\t"<<clusTX<<"\t"<<clusTY<<"\t"<<X0<<"\t"<<X1<<"\t"<<Y0<<"\t"<<Y1<<"\t"<<pixelX<<"\t"<<pixelY<<endl;
//				}    

    } else if (event.tZero.check()) {// Check for T0 event.
           
           cout<<" T0 events" <<endl;
           
    } else if (event.generic.check()) { // match all other events and notify.
      cout << "Unexpected event type ID=" << event.generic.id << " loaded.\n"<<endl;
    } else { // if we were to get to here, must be a corrupt event
      cout << "Corrupt event detected.\n";
    }
  }
  
  
  if (numWordsSkipped > 0) {
    cout << SKIP_WORD_SIZE * numWordsSkipped
                    << " bytes of file data were skipped when locating valid events.\n";
  }
  cout << "Finished loading a file.\n"<<endl;

		fclose(fp);
		ofile.close();


		return 1;
	
}

