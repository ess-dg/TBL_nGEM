// Mantid Repository : https://github.com/mantidproject/mantid

#include <inttypes.h>
#include <string>

 uint64_t CONTIN_ID_VALUE = 0x4F;
 uint64_t EVENT_ID_MASK = 0x40;

/// Generic event to separate bits.
struct GenericEvent {
  uint64_t t0id : 24;      // T0 ID
  uint64_t reserved2 : 32; // Reserved for non-generics
  uint64_t contin : 8;     // 0x4F Continuation Code
  uint64_t reserved1 : 56; // Reserved for non-generics
  uint64_t id : 8;         // Event ID
  bool check() const {
    // as id is 8 bit, we can do a simple AND to check
    return (id & EVENT_ID_MASK) != 0 && contin == CONTIN_ID_VALUE;
  }
};

/// Indicate time 0, the start of a new frame.
struct T0FrameEvent {
  uint64_t t0id : 24;       // T0 ID
  uint64_t eventCount : 32; // Event Count
  uint64_t contin : 8;      // 0x4F Continuation Code
  uint64_t totalLoss : 24;  // Total loss count
  uint64_t eventLoss : 20;  // Event loss count
  uint64_t frameLoss : 12;  // Frame loss count
  uint64_t id : 8;          // 0x4E Event ID
  static constexpr int T0_IDENTIFIER = 0x4E;
  bool check() const { return id == T0_IDENTIFIER && contin == CONTIN_ID_VALUE; }
};

/// A detected neutron.
struct CoincidenceEvent {
  uint64_t t0id : 24;         // T0 ID
  uint64_t clusterTimeY : 10; // Integrated time of the cluster on the Y side
                              // (5ns pixel)
  uint64_t timeDiffY : 6;     // Time lag from first to last detection on Y (5ns)
  uint64_t clusterTimeX : 10; // Integrated time of the cluster on the X side
                              // (5ns pixel)
  uint64_t timeDiffX : 6;     // Time lag from first to last detection on X (5ns)
  uint64_t contin : 8;        // 0x4F Continuation Code
  uint64_t lastY : 7;         // Y position of pixel detected last
  uint64_t firstY : 7;        // Y position of pixel detected first
  uint64_t lastX : 7;         // X position of pixel detected last
  uint64_t firstX : 7;        // X position of pixel detected first
  uint64_t timeOfFlight : 28; // Difference between T0 and detection (1ns)
  uint64_t id : 8;            // 0x47 Event ID.

  uint64_t avgX() const { return (firstX + lastX) / 2; }
  uint64_t avgY() const { return (firstY + lastY) / 2; }
  static constexpr int COINCIDENCE_IDENTIFIER = 0x47;
  bool check() { return id == COINCIDENCE_IDENTIFIER && contin == CONTIN_ID_VALUE; }
  uint64_t getPixel() const {
    return avgX() + (avgY() << 7); // Increase Y significance by 7 bits to
                                   // account for 128x128 grid.
  }
};

/// Holds the 128 bit words from the detector.
struct DetectorWord {
  uint64_t words[2]; // Array holding the word from the detector split in two.
};

/// Is able to hold all versions of the data words in the same memory location.
union EventUnion {
  GenericEvent generic;
  T0FrameEvent tZero;
  CoincidenceEvent coincidence;
  DetectorWord splitWord;
};
