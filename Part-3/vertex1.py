#!/usr/bin/env python3
import sys
import shlex
import cmd
import re
import math

class Vertex_Cover_Class(cmd.Cmd):
    prompt = ''
    use_rawinput = 0

    def __init__(self):
        cmd.Cmd.__init__(self, completekey=None)
        self.Graph = Graph_Generator_Class()
    
    def parse_input(self, input_line):

        input_line = input_line.strip()
        if not input_line:
            return None, None, input_line
       
        i = 0
        while i < len(input_line) and input_line[i] != ' ': i = i+1
        cmd = input_line[:i]
        arg = input_line[i:].strip()
        return cmd, arg, input_line
    
    def do_add(self, args):
        args_= (ParseLine(args, 'do_add'))
        if args_:
            self.Graph.add_street(*args_)

    def do_rm(self, args):
        args_= (ParseLine(args, 'do_rm'))
        if args_:
            self.Graph.rm_street(*args_)

    def do_mod(self, args):
        args_ = (ParseLine(args, 'do_mod'))
        if args_:        
            self.Graph.mod_street(*args_)

    def do_gg(self, args):
        if not args:
            self.Graph.gg_graph()
            print(self.Graph)
        else:
            print('Error: Invalid input arguments', file=sys.stderr)

    def postcmd(self, stop, input_line):
        if(not input_line or input_line =='EOF'):
            stop = True
        return stop
    
    def default(self, input_line):
        if input_line != 'EOF':
            print('Error: the input command "{0}" is not valid'.format(input_line), file=sys.stderr)

    
class Graph_Generator_Class(object):
    """in this class we implement add, mod, rm and gg functions according to the assignment by finding intersection nodes
    then graph printer method can print our graph """

    def __init__(self):
        self.graph_list = {}
        self.vertices_list = {}
        self.edges_list = set([])
        self.intersections = set([])

    def __str__(self):
        v_str = {vid: idx + 1 for idx, vid in enumerate(self.vertices_list.values())}
        out_string = 'V {0}\n'.format(len(self.vertices_list))
        out_string += 'E {'
        if self.edges_list:
            for edge in self.edges_list:
                item = list(edge)
                v_1 = v_str[item[0]]
                v_2 = v_str[item[1]]
                out_string += '<{0},{1}>,'.format(v_1, v_2)
            out_string = out_string[:-1] + '}'
        else:
            out_string += '}'

        return out_string

    def add_street(self, street, vertices):
    
        if vertices:
            if street in self.graph_list:
                print('Error: add specified for the street {0} that exists in list'.format(street), file=sys.stderr)
            else:
                self.graph_list[street] = vertices
                return True
        else:
            print('Error: add command does not have specified vertices', file=sys.stderr)

        return False

    def mod_street(self, street, vertices):
        
        if vertices:
            if street in self.graph_list:
                self.graph_list[street] = vertices
                return True
            else:
                print('Error: mod specified for the street {0} that does not exist'.format(street), file=sys.stderr)
        else:
            print('Error: mod command does not have specified vertices', file=sys.stderr)

        return False

    def rm_street(self, street, *args):
        
        if street in self.graph_list:
            del self.graph_list[street]
            return True
        else:
            print('Error: rm specified for a street {0} that does not exist'.format(street), file=sys.stderr)
        
        return False

    def gg_graph(self):

        self.edges_list = set([])
        graph_2 = {}
        intersections_2 = set([])

        for s_1, v_1 in self.graph_list.items():
            graph_2[s_1] = []
            for i in range(len(v_1)-1):
                my_list = []      
                for s_2, v_2 in self.graph_list.items():
                    if s_1 != s_2:
                        for j in range(len(v_2)-1):
                            int_node = self.intersect_measure(v_1[i], v_1[i+1], v_2[j], v_2[j+1])
                            if int_node:
                                [intersections_2.add(item) for item in int_node]
                                [my_list.append(item) for item in int_node if (item != v_1[i] and item != v_1[i+1])]
                        
                if (v_1[i] in intersections_2 or v_1[i+1] in intersections_2  or len(my_list) > 0 
                        or (graph_2[s_1] or [None])[-1] in intersections_2): 
                    graph_2[s_1].append(v_1[i])

                if len(my_list) > 1:
                    my_list = list(set(my_list)) 
                    node_distance = [math.sqrt((node[0] - v_1[i][0]) ** 2 + (node[1] - v_1[i][1]) ** 2) for node in my_list]
                    node_distance, my_list = zip(*sorted(zip(node_distance, my_list)))
                for p in my_list:
                    graph_2[s_1].append(p)

            if (graph_2[s_1] or [None])[-1] in intersections_2:
                graph_2[s_1].append(v_1[-1])
        
        del_list = set([])
        for s, v in graph_2.items():
            [del_list.add(x) for x in v]
        del_list = set(self.vertices_list.keys()) - intersections_2
        {self.vertices_list.pop(x) for x in del_list}
        self.intersections = intersections_2

        i = 1
        for s_, v_ in graph_2.items():
            p_n = None
            for id_, p in enumerate(v_):
                if p not in self.vertices_list:
                    while i in self.vertices_list.values():
                        i +=1
                    self.vertices_list[p] = i

                vid = self.vertices_list[p]
                if(id_ > 0 and p in self.intersections or p_n in self.intersections):
                    self.edges_list.add(frozenset([vid, self.vertices_list.get(p_n)]))
                p_n = p
        return

    @staticmethod
    def intersect_measure(v_1, v_2, v_3, v_4):
        
        x1, y1 = v_1[0], v_1[1]
        x2, y2 = v_2[0], v_2[1]
        x3, y3 = v_3[0], v_3[1]
        x4, y4 = v_4[0], v_4[1]

        xmin_1, xmax_1 = min(x1,x2), max(x1,x2)
        xmin_2, xmax_2 = min(x3,x4), max(x3,x4)
        ymin_1, ymax_1 = min(y1,y2), max(y1,y2)
        ymin_2, ymax_2 = min(y3,y4), max(y3,y4)
        
        x_int = (max(xmin_1, xmin_2), min(xmax_1, xmax_2))
        y_int = (max(ymin_1, ymin_2), min(ymax_1, ymax_2))

        if x1 == x2 == x3 == x4:
            p_list = [v_1,v_2,v_3,v_4]
            intersections = []
            for p in p_list:
                if y_int[0] <= p[1] <= y_int[1]:
                    intersections.append(p)
            return intersections

        elif x1 != x2 and x3 != x4:
            m1 = (y2-y1)/(x2-x1)
            b1 = y1-m1*x1
            m2 = (y4-y3)/(x4-x3)
            b2 = y3-m2*x3   
            
            if m1 == m2 and b1 == b2:
                p_list = [v_1,v_2,v_3,v_4]
                intersections = []
                for p in p_list:
                    if (x_int[0] <= p[0] <= x_int[1] and y_int[0] <= p[1] <= y_int[1]):
                        intersections.append(p)
                return intersections

        xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4))
        xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))

        ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4)
        yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4)

        try:
            x_val =  xnum / xden    
            y_val = ynum / yden
        except ZeroDivisionError:
            return []

        if (x_val < x_int[0] or x_val> x_int[1] or y_val < y_int[0] or y_val > y_int[1]):
            return []

        return [(round(x_val,2), round(y_val,2))]


    
def ParseLine(args, func):
    if not args:
        print('Error: invalid input arguments', file=sys.stderr)
        return False
    try:
        tmp = shlex.split(args)
    except:
        print('Error: Invalid input for parse', file=sys.stderr)
        return False
   
    street = tmp[0].lower()
    if re.search(r'[^A-Za-z0-9 ]', street):
        print('Error: Invalid character for street name', file=sys.stderr)
        return False

    if len(tmp) > 1:
        if func == 'do_rm':
            print('Error: too many arguments for rm command', file=sys.stderr)
            return False
        vertices = ''.join(tmp[1:])
        if re.search(r'[^0-9,\(\)\- ]', vertices):
            print('Error: Invalid character for vertices', file=sys.stderr)
            return False
        
        open_p = vertices.count('(')
        close_p = vertices.count(')')
        if open_p != close_p:
            print('Error:  missing parenthesis for vertices', file=sys.stderr)
            return False

        regex = r'\((.*?)\)+?'
        vertices = re.findall(regex, vertices)
        p_vertices = []
        
        try:        
            for vertex in vertices:
                c = vertex.split(',')
                if len(c) != 2:
                    raise Exception
                p_vertices.append(tuple([int(x) for x in c]))  
        except:
            print('Error: invalid Vertices for parse', file=sys.stderr)
            return False
        
        if (len(p_vertices) == 0 or
            len(p_vertices) != open_p):
            
            print('Error: invalid vertices', file=sys.stderr)
            return False

        return [street, p_vertices]
    
    else:
        return [street, None]
 
    return False


def main(args):
    graph = Vertex_Cover_Class()
    graph.cmdloop()

    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))

'''
test inputs:
add "Weber Street" (2,-1) (2,2) (5,5) (5,6) (3,8)
add "King Street S" (4,2) (4,8)
add "Davenport Road" (1,4) (5,8)
gg
mod "Weber Street" (2,1) (2,2)
gg
rm "King Street S"
gg
'''
