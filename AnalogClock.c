/*
 * AnalogClock.c
 * 
 * Copyright 2018 Gabriel Izoton <gabrielizotongo@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */

#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

void plotLineAngle(SDL_Renderer* renderer, int xs, int ys, double degrees, double magnitude);
void drawCircle(SDL_Renderer* renderer, int x, int y, int steps, double radius);
void drawHours(SDL_Renderer* renderer, int x, int y, double radius1, double radius2);

const float degtorad = (3.14159265359 * 2) / 360;
const int SCREEN_WIDTH = 360;
const int SCREEN_HEIGHT = 360;

int main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    
    time_t t;
    struct tm tm;
    
    int hour;
	int minute;
	int second;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    while (1) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }
		
		t = time(NULL);
		tm = *localtime(&t);
		
		hour = tm.tm_hour;
		minute = tm.tm_min;
		second = tm.tm_sec;
		
		
        /*Draw background*/
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);
        
        /*Draw clock wireframe*/
        SDL_SetRenderDrawColor(renderer, 0x00, 0x88, 0xFF, 0xFF);
        drawCircle(renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 30, 170);
        drawCircle(renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, 140);
        
        /*Draw hours*/
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
        drawHours(renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 145, 165);
        
        /*Draw hour hand*/
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        plotLineAngle(renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, hour * 30 - 90, 120);
        
        /*Draw minute hand*/
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
        plotLineAngle(renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, minute * 6 - 90, 150);
        
        /*Draw second hand*/
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
        plotLineAngle(renderer, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, second * 6 - 90, 165);
        
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}

void drawCircle(SDL_Renderer* renderer, int x, int y, int steps, double radius){
	
	int i;
	double angleDelta = (steps >= 3) ? 360.0/steps : 120;
	
	for (i = 0; i < steps; i++) {
		
		double endx = radius * cos (i * angleDelta * degtorad);
		double endy = radius * sin (i * angleDelta * degtorad);
		
		double endxNext = radius * cos ((i + 1) * angleDelta * degtorad);
		double endyNext = radius * sin ((i + 1) * angleDelta * degtorad);
		
		SDL_RenderDrawLine(renderer, endx + x, endy + y, endxNext + x, endyNext + y);
	}
	
}

void drawHours(SDL_Renderer* renderer, int x, int y, double radius1, double radius2){
	
	int i;
	
	for (i = 0; i < 360; i+= 30) {
		
		double endx = radius1 * cos (i * degtorad);
		double endy = radius1 * sin (i * degtorad);
		
		double endx2 = radius2 * cos (i * degtorad);
		double endy2 = radius2 * sin (i * degtorad);
		
		SDL_RenderDrawLine(renderer, endx + x, endy + y, endx2 + x, endy2 + y);
	}
	
}

void plotLineAngle(SDL_Renderer* renderer, int xs, int ys, double degrees, double magnitude) {
	
	double w = magnitude * cos (degrees * degtorad);
    double h = magnitude * sin (degrees * degtorad);
    SDL_RenderDrawLine(renderer, xs, ys, xs+(int)w, ys+(int)h);
    
}
