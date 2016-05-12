# pairwisedistancesplugin

PairwiseDistancesPlugin class is a plugin for gama-q2 adjustment program from Qt based GUI of project GNU gama https://www.gnu.org/software/gama/.  To build the plugin you need QtCreator or Qt platform with Cmake installed and source codes from GNU gama project, the plugin is build as a Qt subproject. 

* download GNU gama sources from git

        git clone git://git.sv.gnu.org/gama/qt.git
        cd  qt/gama-q2/lib/
        git clone git://git.sv.gnu.org/gama.git

* go to qt/gama-q2-plugins

        cd  ../../gama-q2-plugins

* clone pairwisedistancesplugin and add it as a subproject to gama-q2.pro

        git clone https://github.com/Celtian/pairwisedistancesplugin/
        # add line "SUBDIRS += gama-q2-plugins/pairwisedistancesplugin" to qt/gama-qt.pro
        
* build project qt/gama-qt.pro

Enjoy!
