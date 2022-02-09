import unittest
from vertex1 import *


class V_C_program_Test(unittest.TestCase):

    def test_ParsLine(self):
        input_line = 'add "Weston St Paul" (2,2) (1,3) (5,6)'
        cmd, street, vertices = ParseLine(input_line)
        self.assertEqual(cmd, 'add')
        self.assertEqual(street, 'weston st paul')
        self.assertEqual(vertices, [(2, 2), (1, 3), (5, 6)])

        input_line = 'gg'
        cmd, street, vertices = ParseLine(input_line)
        self.assertEqual(cmd, 'gg')
        self.assertEqual(street, None)
        self.assertEqual(vertices, None)

        input_line = 'rm "Weston St Paul"'
        cmd, street, vertices = ParseLine(input_line)
        self.assertEqual(cmd, 'rm')
        self.assertEqual(street, 'weston st paul')
        self.assertEqual(vertices, None)

        input_line = 'mod "Weston St Paul" (2,2) (1,2)'
        cmd, street, vertices = ParseLine(input_line)
        self.assertEqual(cmd, 'mod')
        self.assertEqual(street, 'weston st paul')
        self.assertEqual(vertices, [(2, 2), (1, 2)])

        with self.assertRaises(Exception):
            input_line = 'MOD "Weston St Paul"'
            cmd, street, vertices = ParseLine(input_line)

            input_line = 'add "Weston St Paul"'
            cmd, street, vertices = ParseLine(input_line)

            input_line = 'add "Weston St Paul" (1,2,3)(2,3)(5,6)'
            cmd, street, vertices = ParseLine(input_line)

            input_line = 'rm'
            cmd, street, vertices = ParseLine(input_line)

            input_line = 'add "Weston St Paul"'
            cmd, street, vertices = ParseLine(input_line)

            input_line = 'add "Weston St Paul" (1,2)(2,3) 5,6)'
            cmd, street, vertices = ParseLine(input_line)

            input_line = 'mod "Weston St Paul"'
            cmd, street, vertices = ParseLine(input_line)

            input_line = 'add "Weston St Paul" (12)(2,3)(5,6)'
            cmd, street, vertices = ParseLine(input_line)

            input_line = 'gg "Weston St Paul"'
            cmd, street, vertices = ParseLine(input_line)

            input_line = 'add "Weston St Paul" (1.0,3)(2,3)(5,6)'
            cmd, street, vertices = ParseLine(input_line)

            input_line = 'mod "Weston St Paul (1,3)(2,3)(5,6)'
            cmd, street, vertices = ParseLine(input_line)

            input_line = 'add "Weston St Paul" (1,3)(2,3)(5,6)'
            cmd, street, vertices = ParseLine(input_line)

    def test_Graph_Generator_Class(self):
        graph = Graph_Generator_Class()

        graph.add_street('St Paul', [(1, 2), (4, 1)])
        graph.add_street('Weber', [(0, 0), (4, 0)])
        output1 = graph.streets_list
        output2 = set(graph.streets_list.keys())
        self.assertEqual(output1, {'St Paul': [(1, 2), (4, 1)], 'Weber': [(0, 0), (4, 0)]})
        self.assertEqual(output2, {'St Paul', 'Weber'})

        graph.add_street('King', [(0, 4), (4, 4)])
        graph.add_street('Albert', [(0, 0), (4, 1)])
        graph.gg_graph()
        output = set(graph.vertices_list.keys())
        expected_output = {(4, 0), (1, 2), (4, 1), (0, 0)}
        print(graph)
        self.assertEqual(output, expected_output)

        graph.add_street('Ring', [(-1, 5), (-1, 0)])
        graph.add_street('Weston', [(-1, 2), (-1, 6)])
        graph.gg_graph()
        output = set(graph.vertices_list.keys())
        graph.graph_printer()
        self.assertEqual(output, {(4, 0), (1, 2), (0, 0), (-1, 0), (-1, 6), (-1, 2), (4, 1), (-1, 5)})

    def test_intersect_measure(self):
        graph = Graph_Generator_Class()

        output = graph.intersect_measure((0, 0), (4, 4), (1, 1), (5, 5))
        self.assertEqual(output, [(4, 4), (1, 1)])

        output = graph.intersect_measure((0, 5), (5, 5), (0, 0), (5, 5))
        self.assertEqual(output, [(5, 5)])

        output = graph.intersect_measure((0, 0), (4, 4), (-2, -2), (5, 5))
        self.assertEqual(output, [(0, 0), (4, 4)])

        output = graph.intersect_measure((0, 4), (4, 4), (0, 0), (5, 4))
        self.assertEqual(output, [])

        output = graph.intersect_measure((0, 0), (2, 2), (2, 0), (0, 2))
        self.assertEqual(output, [(1, 1)])


if __name__ == '__main__':
    unittest.main()
