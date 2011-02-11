// This is the header file just for the BufferOverFlowSecurityFlaw, other header files would be 
// defined, one for each security flaw. The attempt is to define a scalable system for handling 
// lots of security flaws.

class BufferOverFlowSecurityFlaw : public SecurityFlaw
   {
  // This is the class defined for the detection of potential buffer overflow vulnerabilities 
  // and seeding of

     public:
       // *********************
       // Vulnerability classes
       // *********************

       // Note that there can be many vulnerabilities for a single security flaw (arrays indexing inside
       // a loop body, array indexing in a conditional test, indexing of a STL vector, etc.).
       // In general, vulnerabilities can be independent of the ways in which a security flaw can be seeded,
       // so there is no one-to-one mapping of SecurityFlaw::Vulnerability derived classes to 
       // SecurityFlaw::SeedSecurityFlaw classes. Each class abstracts the details of detecting a specific
       // kind of vulnerability (potential security vulnerability), there can be many classes.
          class BufferOverflowVulnerability_InLoop : public SecurityFlaw::Vulnerability
             {
               public:
                 // This class is only required where the specific security flaw's vulnerability detection requires it.
                 // Since this cases is detecting buffer overflow optional vunlerabilities in loops we use an inherited 
                 // attribute to detect when we are in a loop.
                    class InheritedAttribute
                       {
                      // This class defines the constraints under which a vulnerability is defined. In this case
                      // we detect buffer overflow (or under flow) vulnerabilities that appear in loops.  The 
                      // reason for this degree of specificity is that that such vulnerabilities can be seeded 
                      // in a wide range of ways (e.g. modifying the loop bounds, in addition to the modifications 
                      // to the array subscript expression).
                         public:
                              bool isLoop;
                              InheritedAttribute() : isLoop(false) {}

                           // Note that we don't really need a copy constructor, since the one generated by C++ 
                           // would work in this limited case, it is just a safe practice.
                              InheritedAttribute(const InheritedAttribute & X) : isLoop(X.isLoop) {}
                       };

                 // This is the ROSE AST traversal mechanism (see ROSE Tutorial for details).
                    class Traversal : public SgTopDownProcessing<InheritedAttribute>
                       {
                         public:
                              SecurityFlaw::Vulnerability* vulnerabilityPointer;

                           // This function defines how to recognise the vulnerability (array accesses only in loops)
                              InheritedAttribute evaluateInheritedAttribute ( SgNode* astNode, InheritedAttribute inheritedAttribute );

                              Traversal(SecurityFlaw::Vulnerability* vulnerabilityPointer );
                       };

            // This overloads a base class virtual function and abstracts the details of setting up a call to the 
            // AST traversal (a few lines of code).
               void detector( SgProject *project );

               std::string get_name() { return "BufferOverflowVulnerability_InLoop"; }
               std::string get_color(){ return "blue1"; }
             };

          class BufferOverflowVulnerability_ExternalToLoop : public SecurityFlaw::Vulnerability
             {
               public:
                 // This class is only required where the specific security flaw's vulnerability detection requires it.
                 // Since this cases is detecting buffer overflow optional vunlerabilities in loops we use an inherited 
                 // attribute to detect when we are in a loop.
                    class InheritedAttribute
                       {
                      // This class defines the constraints under which a vulnerability is defined. In this case
                      // we detect buffer overflow (or under flow) vulnerabilities that appear in loops.  The 
                      // reason for this degree of specificity is that that such vulnerabilities can be seeded 
                      // in a wide range of ways (e.g. modifying the loop bounds, in addition to the modifications 
                      // to the array subscript expression).
                         public:
                              bool isLoop;
                              InheritedAttribute() : isLoop(false) {}

                           // Note that we don't really need a copy constructor, since the one generated by C++ 
                           // would work in this limited case, it is just a safe practice.
                              InheritedAttribute(const InheritedAttribute & X) : isLoop(X.isLoop) {}
                       };

                 // This is the ROSE AST traversal mechanism (see ROSE Tutorial for details).
                    class Traversal : public SgTopDownProcessing<InheritedAttribute>
                       {
                         public:
                              SecurityFlaw::Vulnerability* vulnerabilityPointer;

                           // This function defines how to recognise the vulnerability (array accesses only in loops)
                              InheritedAttribute evaluateInheritedAttribute ( SgNode* astNode, InheritedAttribute inheritedAttribute );

                              Traversal(SecurityFlaw::Vulnerability* vulnerabilityPointer );
                       };

            // This overloads a base class virtual function and abstracts the details of setting up a call to the 
            // AST traversal (a few lines of code).
               void detector( SgProject *project );

               std::string get_name() { return "BufferOverflowVulnerability_ExternalToLoop"; }
               std::string get_color(){ return "green"; }
             };


       // ***************
       // Seeding classes
       // ***************

       // This class abstracts the operations about seeding (there could be more than one class).
          class SeedBufferOverflowSecurityFlaw_ModifyArrayIndex : public SecurityFlaw::SeedSecurityFlaw
             {
            // This class introduces a single kind of seeding at either a specific grainularity 
            // (file, function, block, statement) or using a specific mechanism to hide it as a 
            // security flaw (hidden behind modification of array indexing, or behind a modification 
            // to the loop bound, etc.). Set the grainularity via the 
            // SeedSecurityFlaw::GrainularitySpecification type member variable seedGrainulatity.

               public:
                 // This is the ROSE AST traversal mechanism (see ROSE Tutorial for details).
                    class SeedTraversal : public SgSimpleProcessing
                       {
                         public:
                              SecurityFlaw::SeedSecurityFlaw* seedingSecurityFlaw;

                              SeedTraversal(SecurityFlaw::SeedSecurityFlaw* seedingSecurityFlaw );

                           // This function defines how to seed the security flaw at
                           // the previously detected location of the vulnerability
                              void visit( SgNode* node );
                       };

            // Constructor and virtual destructor
               SeedBufferOverflowSecurityFlaw_ModifyArrayIndex();
               virtual ~SeedBufferOverflowSecurityFlaw_ModifyArrayIndex();

            // This overloads a base class virtual function and abstracts the details of setting up a call to the 
            // AST traversal (a few lines of code).
               void seed( SgNode *astNode );

               std::string get_name() { return "SeedBufferOverflowSecurityFlaw_ModifyArrayIndex"; }
             };

          class SeedBufferOverflowSecurityFlaw_AlterLoopBounds : public SecurityFlaw::SeedSecurityFlaw
             {
            // This seeding class modifies loop bounds to cause array accesses in loops to be buffer overflows.
               public:
                    class SeedTraversal : public SgSimpleProcessing
                       {
                         public:
                              SecurityFlaw::SeedSecurityFlaw* seedingSecurityFlaw;

                              SeedTraversal(SecurityFlaw::SeedSecurityFlaw* seedingSecurityFlaw );
                              void visit( SgNode* node );
                       };

               SeedBufferOverflowSecurityFlaw_AlterLoopBounds();
               virtual ~SeedBufferOverflowSecurityFlaw_AlterLoopBounds();

               void seed( SgNode *astNode );

               std::string get_name() { return "SeedBufferOverflowSecurityFlaw_AlterLoopBounds"; }
             };

       // Constructor (constucts all classes derived from SecurityFlaw::Vulnerability and 
       // SecurityFlaw::SeedSecurityFlaw and inserst them into containers in the base class)
          BufferOverFlowSecurityFlaw();

       // Destructor (not really required since there are no data members)
         ~BufferOverFlowSecurityFlaw();
   };

