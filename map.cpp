#include "map.h"

Map::Map(double sseed) { seed = sseed; }

Map::Map(const char * filename, double sseed)
{
    logfilename = filename;
    seed = sseed;
}

Map::~Map() {
    if(grid) {
        for(int c = 0; c < height; c++) {
            delete [] grid[c];
        }
        delete[] grid;
    }
}

int * Map::operator [] (int i) {
    return grid[i];
}
const int * Map::operator [] (int i) const {
    return grid[i];
}

std::vector<std::vector<Point<int> > > Map::get_obstacles() {
    return obstacles;
}

Point<int> Map::convert_abs_point(Point<double> abs_point, Edges new_edge)
{
    Point<int> result;
    abs_point.x -= new_edge.left;
    result.x = floor(abs_point.x / seed);

    abs_point.y -= new_edge.up;
    result.y = floor(abs_point.y / seed);
    return result;
}

Point<int> Map::nearest_int(Point<int> point)
{
    Point<int> result;
    result.x = point.x;
    result.y = point.y;
    if (point.x < 0) result.x = 0;
    if (point.x >= width) result.x = width - 1;
    if (point.y < 0) result.y = 0;
    if (point.y >= height) result.y = height - 1;
    return result;
}

bool Map::in_bounds(Point<int> point)
{
    return point.x >= 0 && point.y >= 0 && point.x < width && point.y < height;
}

void Map::discrete()
{
    double left_edge = abs_start.x - seed / 2;
    if (left_edge < 0) left_edge = 0;
    double right_edge = abs_width - (abs_start.x + seed / 2);
    if (right_edge < 0) right_edge = 0;
    width = static_cast<int>(left_edge / seed) + 1 + static_cast<int>(right_edge / seed);
    left_edge = left_edge - static_cast<int>(left_edge / seed) * seed;
    right_edge = abs_width - (abs_width - right_edge + static_cast<int>(right_edge / seed) * seed);

    double up_edge = abs_start.y - seed / 2;
    if (up_edge < 0) up_edge = 0;
    double down_edge = abs_height - (abs_start.y + seed / 2);
    if (down_edge < 0) down_edge = 0;
    height = static_cast<int>(up_edge / seed) + 1 + static_cast<int>(down_edge / seed);
    up_edge = up_edge - static_cast<int>(up_edge / seed) * seed;
    down_edge = abs_height - (abs_height - down_edge + static_cast<int>(down_edge / seed) * seed);

    Edges edge_(left_edge, right_edge, up_edge, down_edge);
    start = convert_abs_point(abs_start, edge_);
    if (!in_bounds(start)) start = nearest_int(start);
    goal = convert_abs_point(abs_goal, edge_);
    if (!in_bounds(goal)) goal = nearest_int(goal);
    for (auto elem : abs_obstacles) {
        std::vector<Point<int> > ob;
        for (auto point : elem) {
            Point<int> p = convert_abs_point(point, edge_);
            ob.push_back(p);
        }
        obstacles.push_back(ob);
    }
    grid = new int * [height];
    for(int count = 0; count < height; ++count){
        grid[count] = new int [width];
        for (int i = 0; i < width; ++i) {
            grid[count][i] = 0;
        }
    }
    std::cout << "Discretisation completed" << std::endl;
}

void Map::get_map(const char *fname) {
    TiXmlDocument doc(fname);
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
                title = title_value;
            }
            TiXmlElement *twidth = map->FirstChildElement(CNS_ABSWIDTH);
            if(twidth) {
                const char *width_value = twidth->GetText();
                abs_width =  convert_to_double(width_value);
            } else exit_error(CNS_ABSWIDTH);
            TiXmlElement *theight = map->FirstChildElement(CNS_ABSHEIGHT);
            if(theight) {
                const char *height_value = theight->GetText();
                abs_height =  convert_to_double(height_value);
            } else exit_error(CNS_ABSWIDTH);

            TiXmlElement *abs_startx = map->FirstChildElement(CNS_ABSSTARTX);
            if(abs_startx) {
                const char *abs_startx_value = abs_startx->GetText();
                abs_start.x = convert_to_double(abs_startx_value);
                start_point = true;
            } else {
                warning(CNS_ABSSTARTX);
                abs_start.x = 0;
                start_point = false;
            }
            TiXmlElement *abs_starty = map->FirstChildElement(CNS_ABSSTARTY);
            if(abs_starty) {
                const char *starty_value = abs_starty->GetText();
                abs_start.y = convert_to_double(starty_value);
            } else {
                warning(CNS_ABSSTARTY);
                abs_start.y = 0;
            }
            TiXmlElement *finishx = map->FirstChildElement(CNS_ABSFINISHX);
            if(finishx) {
                const char *finishx_value = finishx->GetText();
                abs_goal.x =  convert_to_double(finishx_value);
            } else {
                warning(CNS_ABSFINISHX);
                abs_goal.x = 0;
            }
            TiXmlElement *finishy = map->FirstChildElement(CNS_ABSFINISHY);
            if(finishy) {
                const char *finishy_value = finishy->GetText();
                abs_goal.y =  convert_to_double(finishy_value);
            } else {
                warning(CNS_ABSFINISHY);
                abs_goal.y = 0;
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
                        abs_obstacles.push_back(ps);
                    }
                }
            } else exit_error(CNS_OBSTACLES);
            std::cout << "Reading map sucsses" << std::endl;
        } else exit_error(CNS_MAP);
        logfilename = fname;
    } else exit_error(CNS_ROOT);
}


//function to convert information and create xml file
void Map::create_xml() {
    TiXmlDocument doc;
    TiXmlDeclaration *decl = new TiXmlDeclaration("1.0","UTF-8","");
    doc.LinkEndChild(decl);

    TiXmlElement *root = new TiXmlElement(CNS_ROOT);

    doc.LinkEndChild(root);

    TiXmlElement *map = new TiXmlElement(CNS_MAP);
    TiXmlElement *twidth = new TiXmlElement(CNS_WIDTH);
    TiXmlText *textwidth = convert_to_xml(width);
    twidth->LinkEndChild(textwidth);
    map->LinkEndChild(twidth);

    TiXmlElement *theight = new TiXmlElement(CNS_HEIGHT);
    TiXmlText *textheight = convert_to_xml(height);
    theight->LinkEndChild(textheight);
    map->LinkEndChild(theight);

    TiXmlElement *cellsize = new TiXmlElement(CNS_CELLSIZE);
    TiXmlText *textcellsize = convert_to_xml(seed);
    cellsize->LinkEndChild(textcellsize);
    map->LinkEndChild(cellsize);

    if (start_point) {
        TiXmlElement *startx = new TiXmlElement(CNS_STARTX);
        TiXmlText *textstartx = convert_to_xml(start.x);
        startx->LinkEndChild(textstartx);
        map->LinkEndChild(startx);

        TiXmlElement *starty = new TiXmlElement(CNS_STARTY);
        TiXmlText *textstarty = convert_to_xml(start.y);
        starty->LinkEndChild(textstarty);
        map->LinkEndChild(starty);

        TiXmlElement *finishx = new TiXmlElement(CNS_FINISHX);
        TiXmlText *textfinishx = convert_to_xml(goal.x);
        finishx->LinkEndChild(textfinishx);
        map->LinkEndChild(finishx);

        TiXmlElement *finishy = new TiXmlElement(CNS_FINISHY);
        TiXmlText *textfinishy = convert_to_xml(goal.y);
        finishy->LinkEndChild(textfinishy);
        map->LinkEndChild(finishy);
    }

    TiXmlElement *gr = new TiXmlElement(CNS_GRID);
    for (int i = 0; i < height; ++i){
        TiXmlElement *row = new TiXmlElement(CNS_ROW);
        TiXmlText *textrow = convert_string(grid[i], width);
        row->SetAttribute(CNS_NUMBER, i);
        row->LinkEndChild(textrow);
        gr->LinkEndChild(row);
    }
    map->LinkEndChild(gr);
    root->LinkEndChild(map);

    std::stringstream ss;
    ss << logfilename;
    std::string file_name = ss.str();
    std::stringstream sss;
    sss << height;
    std::string add;
    if (height != width) {
        std::stringstream s;
        s << width;
        add = "_" + sss.str() + "x" + s.str() + ".";
    } else add = "_" + sss.str() + ".";
    if(file_name.rfind(".") != std::string::npos) {
        file_name.replace(file_name.rfind("."), 1, add);
    } else {
        file_name += add + "xml";
    }
    logfilename = file_name;

    doc.SaveFile(logfilename.c_str());
    std::cout << "XML file has been created" << std::endl;
}

void Map::process_map() {
    get_map(logfilename.c_str());
    discrete();
}
