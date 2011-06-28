#include <iostream>
#include <boost/lexical_cast.hpp>
#include <CommandLine.h>
#include <Debug.h>
#include <Globals.h>
#include <CommonNamespaces.h>

CommandLine * CommandLine::globalsInstance = NULL;

/*
 * ======================================================
 * Private functions
 * ======================================================
 */

void
CommandLine::addHelpOption ()
{
  class HelpOption: public CommandLineOption
  {
    public:

      virtual void
      run ()
      {
        CommandLine::getInstance ()->outputOptions ();
      }

      HelpOption (std::string helpMessage, std::string shortOption,
          std::string longOption) :
        CommandLineOption (helpMessage, shortOption, longOption)
      {
      }
  };

  HelpOption * help = new HelpOption ("Display this message", "h", "help");

  addOption (help);
}

void
CommandLine::addVerboseOption ()
{
  class VerboseOption: public CommandLineOption
  {
    public:

      virtual void
      run ()
      {
        Debug::getInstance ()->setVerbose ();
      }

      VerboseOption (std::string helpMessage, std::string shortOption,
          std::string longOption) :
        CommandLineOption (helpMessage, shortOption, longOption)
      {
      }
  };

  VerboseOption * verbose = new VerboseOption ("Be verbose", "v", "verbose");

  addOption (verbose);
}

void
CommandLine::addDebugOption ()
{
  using boost::lexical_cast;
  using std::string;

  class DebugOption: public CommandLineOptionWithParameters
  {
    public:

      virtual void
      run ()
      {
        Debug::getInstance ()->setDebugLevel (getParameter ());
      }

      DebugOption (std::string helpMessage, std::string parameterName,
          std::string shortOption, std::string longOption) :
        CommandLineOptionWithParameters (helpMessage, parameterName,
            shortOption, longOption)
      {
      }
  };

  string const parameterName = "n";

  string const helpMessage = "Set the debug level. Valid values of "
      + parameterName + " are in the range " + lexical_cast <string> (
      Debug::LOWEST_DEBUG_LEVEL) + ".." + lexical_cast <string> (
      Debug::HIGHEST_DEBUG_LEVEL);

  DebugOption * debug = new DebugOption (helpMessage, parameterName, "d",
      "debug");

  addOption (debug);
}

CommandLine::CommandLine ()
{
  addHelpOption ();

  addVerboseOption ();

  addDebugOption ();
}

/*
 * ======================================================
 * Public functions
 * ======================================================
 */

void
CommandLine::outputOptions ()
{
  using std::map;
  using std::string;
  using std::cout;
  using std::endl;

  int const optionExtraSpaces = 2;
  int const parameterExtraSpaces = 3;
  int maxLengthOfShortOption = 0;
  int maxLengthOfLongOption = 0;

  cout << "Options:";

  for (map <string, CommandLineOption *>::iterator it = otherArguments.begin (); it
      != otherArguments.end (); ++it)
  {
    CommandLineOption * option = it->second;

    CommandLineOptionWithParameters * paramterisedOption =
        dynamic_cast <CommandLineOptionWithParameters *> (option);

    if (option->hasShortOption ())
    {
      int length = option->getShortOption ().size () + optionExtraSpaces;

      cout << " [" + option->getShortOption ();

      if (paramterisedOption != NULL)
      {
        length += paramterisedOption->getParameterName ().size ()
            + parameterExtraSpaces;

        cout << " <" + paramterisedOption->getParameterName () + ">";
      }

      cout << "]";

      if (length > maxLengthOfShortOption)
      {
        maxLengthOfShortOption = length;
      }
    }

    if (option->hasLongOption ())
    {
      int length = option->getLongOption ().size () + optionExtraSpaces;

      cout << " [" + option->getLongOption ();

      if (paramterisedOption != NULL)
      {
        length += paramterisedOption->getParameterName ().size ()
            + parameterExtraSpaces;

        cout << " <" + paramterisedOption->getParameterName () + ">";
      }

      cout << "]";

      if (length > maxLengthOfLongOption)
      {
        maxLengthOfLongOption = length;
      }
    }
  }

  cout << endl << endl;

  /*
   * ======================================================
   * Now detail the options
   * ======================================================
   */

  for (map <string, CommandLineOption *>::iterator it = otherArguments.begin (); it
      != otherArguments.end (); ++it)
  {
    CommandLineOption * option = it->second;

    CommandLineOptionWithParameters * paramterisedOption =
        dynamic_cast <CommandLineOptionWithParameters *> (option);

    int length = 0;

    if (option->hasShortOption ())
    {
      length = option->getShortOption ().size () + optionExtraSpaces;

      cout << "[" + option->getShortOption ();

      if (paramterisedOption != NULL)
      {
        length += paramterisedOption->getParameterName ().size ()
            + parameterExtraSpaces;

        cout << " <" + paramterisedOption->getParameterName () + ">";
      }

      cout << "]";
    }

    for (int i = length; i < maxLengthOfShortOption + 1; ++i)
    {
      cout << " ";
    }

    length = 0;

    if (option->hasLongOption ())
    {
      length = option->getLongOption ().size () + optionExtraSpaces;

      cout << "[" + option->getLongOption ();

      if (paramterisedOption != NULL)
      {
        length += paramterisedOption->getParameterName ().size ()
            + parameterExtraSpaces;

        cout << " <" + paramterisedOption->getParameterName () + ">";
      }

      cout << "]";

      for (int i = length; i < maxLengthOfLongOption + 1; ++i)
      {
        cout << " ";
      }
    }

    cout << option->getHelpMessage () + "." << endl;
  }

  exit (0);
}

unsigned int
CommandLine::getNumberOfArguments () const
{
  return ROSEArguments.size ();
}

char **
CommandLine::getArguments () const
{
  using std::string;
  using std::vector;

  /*
   * ======================================================
   * Allocate enough space for all the command-line
   * arguments recognised as ROSE ones
   * ======================================================
   */
  char ** argv = new char*[ROSEArguments.size ()];

  int i = 0;
  for (vector <string>::const_iterator it = ROSEArguments.begin (); it
      != ROSEArguments.end (); ++it)
  {
    /*
     * ======================================================
     * Allocate space for the character array which represents
     * a command-line argument
     * ======================================================
     */
    argv[i] = new char[it->size ()];

    /*
     * ======================================================
     * Have to do a 'strcpy' because 'c_str' returns a pointer
     * to a constant character array and we need a modifiable
     * character array
     * ======================================================
     */
    strcpy (argv[i], (*it).c_str ());

    /*
     * ======================================================
     * Advance the index into argv
     * ======================================================
     */
    ++i;
  }

  return argv;
}

void
CommandLine::parse (int argc, char ** argv)
{
  using std::string;

  CommandLineOptionWithParameters * paramterisedOption = NULL;

  for (int i = 0; i < argc; ++i)
  {
    if (paramterisedOption != NULL)
    {
      /*
       * ======================================================
       * The n-1th option was a parameterised option so grab
       * its parameter
       * ======================================================
       */

      paramterisedOption->setParameter (argv[i]);

      paramterisedOption->run ();

      paramterisedOption = NULL;
    }
    else if (otherArguments.find (argv[i]) == otherArguments.end ())
    {
      /*
       * ======================================================
       * Do not recognise the option so assume it is a ROSE flag
       * ======================================================
       */

      ROSEArguments.push_back (argv[i]);
    }
    else
    {
      /*
       * ======================================================
       * Get the option
       * ======================================================
       */

      CommandLineOption * option = otherArguments[argv[i]];

      paramterisedOption
          = dynamic_cast <CommandLineOptionWithParameters *> (option);

      if (paramterisedOption == NULL)
      {
        option->run ();
      }
    }
  }
}

void
CommandLine::addOption (CommandLineOption * option)
{
  if (option->getShortOption ().empty () == false)
  {
    otherArguments[option->getShortOption ()] = option;
  }
  else
  {
    otherArguments[option->getLongOption ()] = option;
  }
}

void
CommandLine::addUDrawGraphOption ()
{
  class UDrawGraphOption: public CommandLineOption
  {
    public:

      virtual void
      run ()
      {
        Globals::getInstance ()->setOutputUDrawGraphs ();
      }

      UDrawGraphOption (std::string helpMessage, std::string shortOption,
          std::string longOption) :
        CommandLineOption (helpMessage, shortOption, longOption)
      {
      }
  };

  UDrawGraphOption * udraw = new UDrawGraphOption ("Generate uDraw graphs",
      "u", "udraw");

  addOption (udraw);
}

CommandLine *
CommandLine::getInstance ()
{
  if (globalsInstance == NULL)
  {
    globalsInstance = new CommandLine ();
  }
  return globalsInstance;
}
