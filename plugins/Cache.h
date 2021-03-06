#include <libstuff/libstuff.h>
#include "../BedrockPlugin.h"
#include "../BedrockTest.h"

// Declare the class we're going to implement below
class BedrockPlugin_Cache : public BedrockPlugin {
  public:
    // Constructor / Destructor
    BedrockPlugin_Cache();
    ~BedrockPlugin_Cache();

    // Implement base class interface
    virtual string getName() { return "Cache"; }
    virtual void initialize(const SData& args);
    virtual void upgradeDatabase(BedrockNode* node, SQLite& db);
    virtual bool peekCommand(BedrockNode* node, SQLite& db, BedrockNode::Command* command);
    virtual bool processCommand(BedrockNode* node, SQLite& db, BedrockNode::Command* command);
    virtual void test(BedrockTester* tester);

  private:
    // Bedrock Cache LRU map
    class LRUMap {
      public:
        // Constructor / Destructor
        LRUMap();
        ~LRUMap();

        // Tests if anything is in the map
        bool empty();

        // Mark a name as being the most recently used (MRU)
        void pushMRU(const string& name);

        // Remove the name that is the least recently used (LRU)
        string popLRU();

      private:
        // A single entry being tracked
        struct Entry {
            // Attributes
            string name;
            list<Entry*>::iterator listIt;
            map<string, Entry*>::iterator mapIt;
        };

        // Attributes
        recursive_mutex _mutex;
        list<Entry*> _lruList;
        map<string, Entry*> _lruMap;
    };

    // Constants
    const int64_t _maxCacheSize;
    LRUMap _lruMap;
};
