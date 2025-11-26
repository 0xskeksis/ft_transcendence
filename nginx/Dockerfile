FROM node:23-alpine AS frontend-build

WORKDIR /app

COPY ../frontend/package.json ../frontend/package-lock.json ./
RUN npm ci
COPY ../frontend .
RUN npm run build

FROM nginx:alpine

COPY certs /certs

# COPY --from=front-build /app/dist /usr/share/nginx/html

EXPOSE 443
CMD ["nginx", "-g", "daemon off;"]
