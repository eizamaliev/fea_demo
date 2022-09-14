#!/usr/bin/env python3

from PIL import Image, ImageDraw
import argparse
import re
import sys
import math

class ColorSpace:
    def __init__(self, array):
        self.range = min(array), max(array);

    
    def __call__(self, value):
        if self.range[0] == self.range[1]:
            return (128, 128, 128);
        v = (value - self.range[0]) / (self.range[1] - self.range[0])
        color = ((v - 0.5)*4.0, 2.0 - abs(v- 0.5)*4.0,	2.0 - v*4.0)
        color = (int(color[0]*255), int(color[1]*255), int(color[2]*255))
        return colori

class Geometry:
    def __init__(self, gridfile, deformation_file="", stressfile=""):
        self.nodes = {}
        self.elements = []
        self.deformations = {}
        self.stress = {}
        self.load_grid(gridfile)
        self.load_stress(stressfile)
        self.load_deformation(deformation_file)
    
    def load_grid(self, filename):
        with open(filename) as file:
            isParsingNodes = False
            isParsingElements = False
            for line in file:
                if line[0] == "$":
                    continue
                if "*NODE" in line:
                    isParsingNodes = True
                    continue
                if "*ELEMENT_SHELL" in line:
                    isParsingElements = True
                    isParsingNodes = False
                    continue
                if isParsingNodes:
                    try:
                        id, x, y = " ".join(line.split()).split()[:3]
                        id = int(id)
                        x = float(x)
                        y = float(y)
                    except:
                        isParsingNodes = False
                        continue
                    self.nodes[id] = (x, y)
                    continue
                if isParsingElements:
                    try:
                        _, _, i, j, k = " ".join(line.split()).split()[:5]
                        i = int(i)
                        j = int(j)
                        k = int(k)
                    except:
                        isParsingElements = False
                        continue
                    self.elements.append((i, j, k))
                    continue

    def load_deformation(self, filename):
        with open(filename) as file:
            
            for line in file:
                id, ux, uy = line.split()
                self.deformations[int(id)] = (float(ux), float(uy))
    
    def load_stress(self, filename):
        with open(filename) as file:
            content = [line for line in file]

        names = content[0].split()
        for name in names:
            self.stress[name] = []

        for line in content[1:]:
            values = line.split()
            for name, value in zip(names, values):
                self.stress[name].append(float(value))


    def apply_deformation(self):
        for id, deformation in self.deformations.items():
            self.nodes[id] = [x + dx for x, dx in zip(self.nodes[id], deformation)]

    def get_size(self):
        size = [list(self.nodes.values())[0][0], list(self.nodes.values())[0][1]] * 2
        for x, y in self.nodes.values():
            if size[0] > x: size[0] = x
            if size[2] < x: size[2] = x
            if size[1] > y: size[1] = y
            if size[3] < y: size[3] = y
        return size




class Visualizer:
    def __init__(self, nodes, elements, stress, suffix=""):
        self.nodes = nodes
        self.elements = elements
        self.stress = stress
        self.suffix = f"_{suffix}" if suffix else ""
        self.w, self.h = (1024,1024)
        self.scale = self.get_scale()

    def get_scale(self):
        size = [list(self.nodes.values())[0][0], list(self.nodes.values())[0][1]] * 2
        for x, y in self.nodes.values():
            if size[0] > x: size[0] = x
            if size[2] < x: size[2] = x
            if size[1] > y: size[1] = y
            if size[3] < y: size[3] = y

        scale = max(abs(size[3] - size[1]) / (0.9*self.h), abs(size[2] - size[0]) / (0.9*self.w))
        return 1/scale

    def default_image(self):
        return Image.new("RGB", (self.w, self.h))

    def draw_grid(self, image=None):
        if image is None:
            print("new image")
            image = self.default_image()
        draw = ImageDraw.Draw(image)
        for element in self.elements:
            i, j, k = element
            lines = self.get_lines(element + (element[0],))
            draw.polygon(lines, width = 1)
        return image

    def draw_stresses(self):
        images = {}

        for stress_name in self.stress.keys():
            colorSpace = ColorSpace(self.stress[stress_name])
            image = self.default_image()
            draw = ImageDraw.Draw(image)
            for element, stress in zip(self.elements, self.stress[stress_name]):
                i, j, k = element
                lines = self.get_lines(element + (element[0],))
                draw.polygon(lines, width = 1, fill=colorSpace(stress))
            images[stress_name] = image
        
        return images

    def get_lines(self, indexes):
        return [(self.scale*self.nodes[i][0] + 0.05*self.w,  self.h - (self.scale*self.nodes[i][1] + 0.05*self.h)) for i in indexes]

    def save(self):
        grid = self.draw_grid()
        grid.save(f"grid{self.suffix}.jpg")

        stress_images = self.draw_stresses()
        for name, image in stress_images.items():
            image = self.draw_grid(image)
            image.save(f"{name}{self.suffix}.jpg")


def build_args():
    parser = argparse.ArgumentParser()

    parser.add_argument('-i', type=str, help='Path to input file.')
    parser.add_argument('-r', type=str, help='Path to result file')
    parser.add_argument('-s', type=str, help='Path to stress file')
    parser.add_argument('-o', type=str, help='Path to output file.')
    return parser.parse_args()



if __name__ == '__main__':
    args = build_args()

    geometry = Geometry(args.i, deformation_file=args.r, stressfile=args.s)
    visualiser = Visualizer(geometry.nodes, geometry.elements, geometry.stress)
    visualiser.save()

    geometry.apply_deformation()
    Visualizer(geometry.nodes, geometry.elements, geometry.stress, "_deformed").save()
