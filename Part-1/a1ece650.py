import math
import re
import shlex
import sys


class Vertex_Cover_Class(object):
    def __init__(self):
        self.Graph = Graph_Generator_Class()

    def add(self, street_name, vertices):
        if vertices:
            self.Graph.add_street(street_name, vertices)
        else:
            raise Exception(" 'add' command does not have specified vertices")

    def mod(self, street_name, vertices):
        if vertices:
            self.Graph.mod_street(street_name, vertices)
        else:
            raise Exception(" 'mod' command does not have specified vertices")

    def rm(self, street_name):
        if street_name:
            self.Graph.rm_street(street_name)

    def gg(self):
        self.Graph.gg_graph()
        self.Graph.graph_printer()


class Graph_Generator_Class(object):
    def __init__(self):
        self.vertices_list = {}
        self.edges_list = set([])
        self.intersections_list = set([])
        self.streets_list = {}

    def add_street(self, street_name, vertices):
        if street_name not in self.streets_list:
            self.streets_list[street_name] = vertices
        else:
            raise Exception(" 'add' specified for a street {} that exists in the street list".format(street_name))

    def rm_street(self, street_name):
        if street_name in self.streets_list:
            del self.streets_list[street_name]
        else:
            raise Exception(" 'rm' specified for a street {} that does not exist in the street list".format(street_name))

    def mod_street(self, street_name, vertices):
        if street_name not in self.streets_list:
            raise Exception(" 'mod' specified for a street {} that does not exist in the street list".format(street_name))
        else:
            self.streets_list[street_name] = vertices

    def gg_graph(self):
        self.edges_list = set([])
        tmp_graph = {}
        tmp_intersections = set([])

        for street, points in self.streets_list.items():
            tmp_graph[street] = []

            # loop through edges of street
            for i in range(len(points)-1):
                tmp_p_to_add = [] #need this list because can have more than one intersection per segement
                
                #Loop through all other streets to find intersections
                for street_2, points_2 in self.streets_list.items():
                    if street != street_2:
                        # loop through other streets segments
                        for j in range(len(points_2)-1):
                            inter_p = self.intersect_measure(points[i], points[i+1], points_2[j], points_2[j+1])
                            if inter_p:
                                [tmp_intersections.add(x) for x in inter_p]
                                [tmp_p_to_add.append(x) for x in inter_p if (x != points[i] and x != points[i+1])]
                        
                # add first point of segement if valid
                if (points[i] in tmp_intersections #first point is an intersection
                        or points[i+1] in tmp_intersections #next point is an intersection
                        or len(tmp_p_to_add) > 0 #there exists an intersection with the segment
                        or (tmp_graph[street] or [None])[-1] in tmp_intersections): #previous point is an intersection
                    tmp_graph[street].append(points[i])

                # insert all intersections by order of distance to segment if more than one
                if len(tmp_p_to_add) > 1:
                    tmp_p_to_add = list(set(tmp_p_to_add)) # remove duplicates
                    tmp_dist = [self.distance(p, points[i]) for p in tmp_p_to_add]
                    tmp_dist, tmp_p_to_add = zip(*sorted(zip(tmp_dist, tmp_p_to_add))) # sort the list by distance
                for tmp_p in tmp_p_to_add:
                    tmp_graph[street].append(tmp_p)

            #add last point if valid
            if (tmp_graph[street] or [None])[-1] in tmp_intersections:
                tmp_graph[street].append(points[-1])
        
        #remove all points from graph that are not in new graph
        to_remove = set([])
        for _, v in tmp_graph.items():
            [to_remove.add(x) for x in v]
        to_remove = set(self.vertices_list.keys()) - tmp_intersections
        {self.vertices.pop(x) for x in to_remove}
        self.intersections_list = tmp_intersections

        # add remaining points that dont yet have an id
        i = 1
        for street, vertices in tmp_graph.items():
            prev = None
            for index, vertex in enumerate(vertices):
                # if vertex doesnt exist, add it
                if vertex not in self.vertices_list:
                    #find an id that isn't used
                    while i in self.vertices_list.values():
                        i +=1
                    self.vertices_list[vertex] = i

                # create edge if valid
                v_id = self.vertices_list[vertex]
                if(index > 0 and (vertex in self.intersections_list or prev in self.intersections_list) ):
                    node_val = self.vertices_list.get(prev)
                    self.edges_list.add((v_id,node_val))
                    # self.edges_list.add(frozenset([v_id, self.vertices_list.get(prev)]))
                prev = vertex

        # self.edges_list = set([])
        # intersections_2 = set([])
        # graph_list2 = {}
        # for s_1, v_1 in self.streets_list.items():
        #     graph_list2[s_1] = []
        #     for i in range(len(v_1)-1):
        #         my_list = []
        #         for s_2, v_2 in self.streets_list.items():
        #             if s_1 != s_2:
        #                 for j in range(len(v_2)-1):
        #                     int_node = self.intersect_measure(v_1[i], v_1[i+1], v_2[j], v_2[j+1])
        #                     if int_node:
        #                         for item in int_node:
        #                             intersections_2.add(item)
        #                             if item != v_1[i+1] and item != v_1[i]:
        #                                 my_list.append(item)
        #         if len(my_list) > 0 or v_1[i] in intersections_2 or v_1[i+1] in intersections_2:
        #             graph_list2[s_1].append(v_1[i])
        #         elif (graph_list2[s_1] or [None])[-1] in intersections_2:
        #             graph_list2[s_1].append(v_1[i])

        #         if len(my_list) > 1:
        #             my_list = list(set(my_list))
        #             for node in my_list:
        #                 node_distance = [math.sqrt((node[0] - v_1[i][0]) ** 2 + (node[1] - v_1[i][1]) ** 2)]
        #                 node_distance, my_list = zip(*sorted(zip(node_distance, my_list)))
        #         [graph_list2[s_1].append(p) for p in my_list]

        #     if (graph_list2[s_1] or [None])[-1] in intersections_2:
        #         graph_list2[s_1].append(v_1[-1])

        # del_list = set([])
        # for s, v in graph_list2.items():
        #     [del_list.add(val) for val in v]
        # del_list = set(self.vertices_list.keys()) - intersections_2
        # {self.vertices_list.pop(item) for item in del_list}    
        # self.intersections_list = intersections_2
        # count = 1
        # for s_, v_ in graph_list2.items():
        #     p_n = None
        #     for id_, p in enumerate(v_):
        #         if p not in self.vertices_list:
        #             while count in self.vertices_list.values():
        #                 count = count + 1
        #             self.vertices_list[p] = count

        #         index = self.vertices_list[p]
        #         if (id_ > 0 and (p_n in self.intersections_list or p in self.intersections_list)):
        #             node_val = self.vertices_list.get(p_n)
        #             self.edges_list.add((index,node_val))
        #             # self.edges_list.add(frozenset([index, self.vertices_list.get(p_n)]))
        #         p_n = p
   
    def graph_printer(self):
        out_string = 'V = {\n'
        for i, j in sorted(self.vertices_list.items(), key=lambda x: x[1]):
            # if type(i[0]) == 'float' or type(i[1]) == 'float':
            #     out_string += "  %s:  (%.2f,%.2f)\n" % (j, i[0], i[1])
            # else:
            out_string += "  %s:  (%.2f,%.2f)\n" % (j, i[0], i[1])
        out_string += '}\nE = {\n'
        if self.edges_list:
            for edge in sorted(self.edges_list):
                out_string += '<{},{}>,\n'.format(edge[0], edge[1])
            out_string = out_string[:-2] + '\n}'
        else:
            out_string += '}'

        print(out_string)

    @staticmethod
    def distance(p1, p2):
        p1x, p1y = p1
        p2x, p2y = p2

        dist = math.sqrt((p1x-p2x)**2 + (p1y-p2y)**2)
        return dist

    @staticmethod
    def intersect_measure(v_1, v_2, v_3, v_4):
        intersect_list = []
        nodes = [v_1, v_2, v_3, v_4]

        x_1, x_2, x_3, x_4 = v_1[0], v_2[0], v_3[0], v_4[0]
        y_1, y_2, y_3, y_4 = v_1[1], v_2[1], v_3[1], v_4[1]

        xmin_1, xmax_1, ymin_1, ymax_1 = min(x_1, x_2), max(x_1, x_2), min(y_1, y_2), max(y_1, y_2)
        xmin_2, xmax_2, ymin_2, ymax_2 = min(x_3, x_4), max(x_3, x_4), min(y_3, y_4), max(y_3, y_4)

        x_int, y_int = (max(xmin_1, xmin_2), min(xmax_1, xmax_2)), (max(ymin_1, ymin_2), min(ymax_1, ymax_2))

        if x_1 != x_2:
            slope_1 = (y_2 - y_1) / (x_2 - x_1)
            y_intercept1 = y_1 - slope_1 * x_1
            if x_3 != x_4:
                slope_2 = (y_4 - y_3) / (x_4 - x_3)
                y_intercept2 = y_3 - slope_2 * x_3
                if y_intercept1 == y_intercept2 and slope_1 == slope_2:
                    for node in nodes:
                        if y_int[0] <= node[1] <= y_int[1] and x_int[0] <= node[0] <= x_int[1]:
                            intersect_list.append(node)
                    return intersect_list
        elif x_1 == x_2 and x_3 == x_4 and x_2 == x_4:
            for node in nodes:
                if y_int[0] <= node[1] <= y_int[1]:
                    intersect_list.append(node)
            return intersect_list

        try:
            y_value = ((x_1 * y_2 - y_1 * x_2) * (y_3 - y_4) - (y_1 - y_2) * (x_3 * y_4 - y_3 * x_4)) / ((x_1 - x_2) * (y_3 - y_4) - (y_1 - y_2) * (
                    x_3 - x_4))
            x_value = ((x_1 * y_2 - y_1 * x_2) * (x_3 - x_4) - (x_1 - x_2) * (x_3 * y_4 - y_3 * x_4)) / (
                    (x_1 - x_2) * (y_3 - y_4) - (y_1 - y_2) * (x_3 - x_4))
        except ZeroDivisionError:
            return []

        if x_int[1] < x_value or x_int[0] > x_value:
            return []
        elif y_int[1] < y_value or y_int[0] > y_value:
            return []

        return [(round(x_value,2), round(y_value,2))]


def ParseLine(input_line):

    try:
        line_ = shlex.split(input_line)
    except:
        raise Exception("Invalid arguments for parse")

    cmd_ = line_[0]
    street_ = None
    vertices_ = None
    if cmd_ == 'gg':
        if len(line_) > 1:
            raise Exception("Too many arguments for 'gg' command")
    elif cmd_ in ('mod', 'add'):
        if len(line_) < 3:
            raise Exception("Invalid arguments for {0} command".format(cmd_))
        street_ = line_[1].lower()
        vertices_ = ''.join(line_[2:])
    elif cmd_ == 'rm':
        if len(line_) > 2:
            raise Exception("Too many arguments for 'rm' command")
        street_ = line_[1].lower()
    else:
        raise Exception("Incorrect input format")

    if street_:
        if re.search(r'[^A-Za-z0-9 ]', str(street_)):
            raise Exception("Invalid characters for street_name")
        if vertices_:
            p_vertices = []
            if re.search(r'[^0-9,\(\)\- ]', str(vertices_)):
                raise Exception("Invalid characters for vertices")

            if vertices_.count('(') != vertices_.count(')'):
                raise Exception("Invalid parenthesis for vertices")

            vertices_ = re.findall(r'\((.*?)\)+?', str(vertices_))
            for item in vertices_:
                ver_pos = item.split(',')
                if len(ver_pos) == 2:
                    p_vertices.append(tuple([int(i) for i in ver_pos]))
                else:
                    raise Exception("Invalid arguments for vertices")

            if len(p_vertices) == 0 and len(p_vertices) != vertices_.count('('):
                raise Exception("Invalid vertices")
            vertices_ = p_vertices

    return cmd_, street_, vertices_


def main():
    graph = Vertex_Cover_Class()
    while True:
        line = sys.stdin.readline()
        print(file=sys.stdout)
        if line == '':
            break
        try:
            cmd, street_n, vertices = ParseLine(line)
            if cmd == 'add':
                graph.add(street_n, vertices)
            elif cmd == 'mod':
                graph.mod(street_n, vertices)
            elif cmd == 'rm':
                graph.rm(street_n)
            elif cmd == 'gg':
                graph.gg()
        except Exception as e:
            print('Error:' + str(e), file=sys.stderr)

    sys.exit(0)


if __name__ == '__main__':
    main()


""" inputs line according to assignment a1:
add "Weber Street" (2,-1) (2,2) (5,5) (5,6) (3,8)
add "King Street S" (4,2) (4,8)
add "Davenport Road" (1,4) (5,8)
gg
mod "Weber Street" (2,1) (2,2)
gg
rm "King Street S"
gg """
