#################################
QML Configuration Editor (part 1)
#################################


This is about developing a simple QML GUI that will aim to configure configuration
parameters.

Consider the following configuration data (YAML, use libyaml):

.. code:: yaml

   categories: # list of categories, used for UI presentation of configuration

   - name: ui
     name[en_US]: UI
     description: User interface misc. Options

   - name: display
     name[en_US]: Display
     parent: ui
     description: Image display options

   keys:
   - key: ui.languages # category is implicit from the key name
     type: list<enum>:
       entries:
       - entry:
           name: en_US
           name[en_US]: English
           name[fr_FR]: Anglais
       - entry:
           name: fr_FR
           name[en_US]: French
           name[fr_FR]: Français
       min: 0 # length of list
       max: 3
     default: [en_US]
     # Example: [fr_FR, de_CH, it_IT] in switzerland

   - key: display.nb_fingers
     type: int
     min: 0
     max: 10
     default: 10
     # here, no name
     description = Number of fingers
     description[fr_FR] = Nombre de doigts

   - key: display.nb_toes
     type: int
     min: 0
     max: 10
     default: 10
     name[en_US]: "# Toes"
     # here, no description



Implement 2 classes for manipulating the configuration:

- Simple configuration reader:

  .. code:: c++

     class ConfigReader {
      private:
        std::map<std::string,int> mMapInt;
        std::map<std::string,int> mMapEnum;
        std::map<std::string,int> mMapStr;
        std::map<std::string,std::list<int>> mMapListInt;
        std::map<std::string,std::list<int>> mMapListEnum;
      public:
        ConfigReader();
      public:
        void load(std::filesystem::path const & cfgfile);
        bool getBool(char const * key);
        int getInt(char const * key);
        std::string getString(char const * key);
        std::list<int> getListEnum(char const * key);
        // ...
     };

- Configuration manager:

  .. code:: c++

     enum class Language {
      en_US,
      fr_FR,
      zh_CN,
     };

     class ConfigManager : public ConfigReader {
      private:
        Language language = en_US;
      public:
        ConfigManager();
      public:
        // Set the language in which pretty names are returned
        void setLanguage(Language lang);

        // Get the graph of categories
        // TBD as nested list? as edges?
        TBD getCategoryTree():

        // Get list of configuration keys in a category
        std::list<std::string> getKeysForCategory(char const * category);

        // Get name of key (translated if language is set)
        std::string getKeyName(char const * key);

        // Get description of key
        std::string getKeyDescription(char const * key);

        KeyType getKeyType(char const * key);

        // :return: list of index, name and description
        std::list<std::tuple<int,std::string,std::string>> getKeyEnumChoices(char const * key);

        // def getKeyIntRange(key: str) -> tuple<int,int>:
        // def setInt(key: str, value: int): // just updates in-memory representation
        // def set*

        void save(std::filesystem::path const & cfgfile);

     };


Implement a QML GUI (using material design) that displays:

- A Tree View on the left

  Nodes are categories/subcategories, leafs are configuration keys.


  The tree should display something analogous to:

  ::

     UI/ <- when selected, right pane shows "user interface misc. options"
      Display/ <- when selected, right pane shows "Image display options"
       nb_fingers <- when selected, right pane shows "Number of
        fingers" or "Nombre de doigts" depending on current display
        language (see ConfigManager::setLanguage())
       # Toes <- no description
         should display either a slider or a number edition widget,
         allowing to adjust value within allowed range


- Dynamic widgets depending on the selected item of the tree view,
  and of the type (and parameters, eg. range) of key

  For example, an int range would be displayed using a slider or spin box,
  an enum would be displayed using a combo box,
  a string would be displayed using a text edit field,
  a list of enum would be displayed using something appropriate.

- A widget allowing to change its language, including
  language of the displayed configuration structure.

- A widget allowing to load from a source file, one to save to a
  destination file.

Notes:

- Use cmake, not qmake
- Use C++14/17
- Use the provided .clang-format
- Configuration editor widget target resolution should be ~ 1600x900
  but it should be reasonably scalable (eg. show something decent with
  a 1280x768 screen resolution or 2560x1440.


