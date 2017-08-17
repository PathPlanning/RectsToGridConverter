#ifndef PARSER
#define PARSER

#include "tinyxml/tinystr.h"
#include "tinyxml/tinyxml.h"
#include "gl.const.h"
#include "point.h"
#include <unordered_map>
#include <sstream>
#include <string>
#include <iostream>
#include <cmath>
#include <random>

//Чтение карты
//функции обработки ошибок
inline void exit_error(const char *tag) {
    std::cout << "ERROR: Reading map failed. Not foung tag '" << tag <<  "'" << std::endl;
    exit(0);
}
inline void warning(const char *tag) {
    std::cout << "WARNING: Not foung tag '" << tag <<  "'. Using default value." << std::endl;
}

template <typename T>
int convert_to_int(T a) {
    std::istringstream myStream(a);
    int uintVar = 0;
    myStream >> uintVar;
    return uintVar;
}

template <typename T>
double convert_to_double(T a) {
    double var;
    var = std::stod(a);
    return var;
}

inline std::string get_info(TiXmlElement *b) {
    if(b) {
        const char *b_value = b->GetText();
        return b_value;
    }
    return "-1";
}

template<typename T>
TiXmlText * convert_to_xml(T a) {
    std::stringstream ss;
    ss << a;
    TiXmlText *text = new TiXmlText(ss.str().c_str());
    return text;
}

template<typename T>
std::string convert_to_string(T a) {
    std::stringstream so;
    so << a;
    std::string c = so.str();
    return c;
}

template<typename T>
const char * convert_to_char(T a) {
    std::ostringstream sc;
    sc << a;
    const char * c = sc.str().c_str();
    return c;
}

inline TiXmlText * convert_string(int * a, int len) {
    std::string st = "";
    for (int i = 0; i < len-1; i++)
        st += convert_to_string(a[i]) + ' ';
    st += convert_to_string(a[len - 1]);
    const char * c = st.c_str();
    TiXmlText *text = new TiXmlText(c);
    return text;
}

template<typename Graph>
void get_map(Graph &_map, char *name) {
    TiXmlDocument doc(name);
    try {
        if(doc.LoadFile()) {
            std::cout << "Load sucsses" << std::endl;
        } else {
            throw "ERROR: Load failed. Try another file";
        }
    }
    catch (const char * exc) {
        std::cout << exc << std::endl;
        exit(0);
    }
    TiXmlElement *root = doc.FirstChildElement(CNS_ROOT);
    if(root) {
        TiXmlElement *map = root->FirstChildElement(CNS_MAP);
        if(map) {
            TiXmlElement *ttitle = map->FirstChildElement(CNS_TITLE);
            if(ttitle) {
                const char *title_value = ttitle->GetText();
                _map.title = title_value;
            }
            TiXmlElement *twidth = map->FirstChildElement(CNS_ABSWIDTH);
            if(twidth) {
                const char *width_value = twidth->GetText();
                _map.abs_width =  convert_to_double(width_value);
            } else exit_error(CNS_ABSWIDTH);
            TiXmlElement *theight = map->FirstChildElement(CNS_ABSHEIGHT);
            if(theight) {
                const char *height_value = theight->GetText();
                _map.abs_height =  convert_to_double(height_value);
            } else exit_error(CNS_ABSWIDTH);

            /*TiXmlElement *seed = map->FirstChildElement(CNS_SEED);
            if(seed) {
                const char *seed_value = seed->GetText();
                _map.seed =  convert_to_double(seed_value);
            } else {
                warning(CNS_SEED);
                _map.seed = 1;
            } */

            TiXmlElement *abs_startx = map->FirstChildElement(CNS_ABSSTARTX);
            if(abs_startx) {
                const char *abs_startx_value = abs_startx->GetText();
                _map.abs_start.x = convert_to_double(abs_startx_value);
                _map.start_point = true;
            } else {
                warning(CNS_ABSSTARTX);
                _map.abs_start.x = 0;
                _map.start_point = false;
            }
            TiXmlElement *abs_starty = map->FirstChildElement(CNS_ABSSTARTY);
            if(abs_starty) {
                const char *starty_value = abs_starty->GetText();
                _map.abs_start.y = convert_to_double(starty_value);
            } else {
                warning(CNS_ABSSTARTY);
                _map.abs_start.y = 0;
            }
            TiXmlElement *finishx = map->FirstChildElement(CNS_ABSFINISHX);
            if(finishx) {
                const char *finishx_value = finishx->GetText();
                _map.abs_goal.x =  convert_to_double(finishx_value);
            } else {
                warning(CNS_ABSFINISHX);
                _map.abs_goal.x = 0;
            }
            TiXmlElement *finishy = map->FirstChildElement(CNS_ABSFINISHY);
            if(finishy) {
                const char *finishy_value = finishy->GetText();
                _map.abs_goal.y =  convert_to_double(finishy_value);
            } else {
                warning(CNS_ABSFINISHY);
                _map.abs_goal.y = 0;
            }
            TiXmlElement *obstacles = map->FirstChildElement(CNS_OBSTACLES);
            if(obstacles) {
                TiXmlElement *obstacle = obstacles->FirstChildElement(CNS_OBSTACLE);
                if(obstacle) {
                    for (obstacle; obstacle; obstacle = obstacle->NextSiblingElement(CNS_OBSTACLE)) {
                        std::vector<Point<double> > ps;
                        TiXmlElement *point = obstacle->FirstChildElement(CNS_POINT);
                        if(point) {
                            for (point; point; point = point->NextSiblingElement(CNS_POINT)) {
                                const char * x = point->Attribute(CNS_X);
                                const char * y = point->Attribute(CNS_Y);
                                ps.push_back(Point<double>(convert_to_double(x), convert_to_double(y)));
                            }
                        }
                        _map.abs_obstacles.push_back(ps);
                    }
                }
            } else exit_error(CNS_OBSTACLES);
            std::cout << "Reading map sucsses" << std::endl;
        } else exit_error(CNS_MAP);
        TiXmlElement *logfilename = root->FirstChildElement(CNS_LOGFILENAME);
        if (logfilename && logfilename->GetText()) {
            _map.logfilename = convert_to_char(get_info(logfilename));
        } else {
            std::cout << "WARNING: Not foung tag '" << CNS_LOGFILENAME <<  "'. Using default value." << std::endl;
            _map.logfilename = name;
        }
    } else exit_error(CNS_ROOT);
}


//Функция вывода в XML
template<typename Graph>
void create_xml(Graph &graph) {
    TiXmlDocument doc;
    TiXmlDeclaration *decl = new TiXmlDeclaration("1.0","UTF-8","");
    doc.LinkEndChild(decl);

    TiXmlElement *root = new TiXmlElement(CNS_ROOT);

    doc.LinkEndChild(root);

    TiXmlElement *map = new TiXmlElement(CNS_MAP);
    TiXmlElement *twidth = new TiXmlElement(CNS_WIDTH);
    TiXmlText *textwidth = convert_to_xml(graph.width);
    twidth->LinkEndChild(textwidth);
    map->LinkEndChild(twidth);

    TiXmlElement *theight = new TiXmlElement(CNS_HEIGHT);
    TiXmlText *textheight = convert_to_xml(graph.height);
    theight->LinkEndChild(textheight);
    map->LinkEndChild(theight);

    TiXmlElement *cellsize = new TiXmlElement(CNS_CELLSIZE);
    TiXmlText *textcellsize = convert_to_xml(graph.seed);
    cellsize->LinkEndChild(textcellsize);
    map->LinkEndChild(cellsize);

    if (graph.start_point) {
        TiXmlElement *startx = new TiXmlElement(CNS_STARTX);
        TiXmlText *textstartx = convert_to_xml(graph.start.x);
        startx->LinkEndChild(textstartx);
        map->LinkEndChild(startx);

        TiXmlElement *starty = new TiXmlElement(CNS_STARTY);
        TiXmlText *textstarty = convert_to_xml(graph.start.y);
        starty->LinkEndChild(textstarty);
        map->LinkEndChild(starty);

        TiXmlElement *finishx = new TiXmlElement(CNS_FINISHX);
        TiXmlText *textfinishx = convert_to_xml(graph.goal.x);
        finishx->LinkEndChild(textfinishx);
        map->LinkEndChild(finishx);

        TiXmlElement *finishy = new TiXmlElement(CNS_FINISHY);
        TiXmlText *textfinishy = convert_to_xml(graph.goal.y);
        finishy->LinkEndChild(textfinishy);
        map->LinkEndChild(finishy);
    }

    TiXmlElement *gr = new TiXmlElement(CNS_GRID);
    for (int i = 0; i < graph.height; ++i){
        TiXmlElement *row = new TiXmlElement(CNS_ROW);
        TiXmlText *textrow = convert_string(graph.grid[i], graph.width);
        //row->SetAttribute(CNS_NUMBER, i);
        row->LinkEndChild(textrow);
        gr->LinkEndChild(row);
    }
    map->LinkEndChild(gr);
    root->LinkEndChild(map);

    std::stringstream ss;
    ss << graph.logfilename;
    std::string file_name = ss.str();
    std::stringstream sss;
    sss << graph.height;
    std::string add = "_" + sss.str() + ".";
    if(file_name.rfind(".") != std::string::npos) {
        file_name.replace(file_name.rfind("."), 1, add);
    } else {
        file_name += add + "xml";
    }
    graph.logfilename = file_name;

    doc.SaveFile(graph.logfilename.c_str());
    std::cout << "XML file has been created" << std::endl;
}

#endif // PARSER

