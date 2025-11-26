FROM node:23-alpine AS build

WORKDIR /app

COPY frontend/package*.json ./

RUN npm ci

COPY . .

EXPOSE 8000
