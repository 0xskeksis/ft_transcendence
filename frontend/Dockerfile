FROM node:23-alpine AS build

WORKDIR /app

# Installer openssl pour générer les certificats
RUN apk add --no-cache openssl

# Copier package.json et installer deps
COPY frontend/package*.json ./
RUN npm install

# Copier tout le frontend
COPY frontend/ .

# Créer les certificats auto-signés
RUN mkdir -p /certs \
 && openssl req -x509 -nodes -days 365 -newkey rsa:2048 \
    -keyout /certs/key.pem \
    -out /certs/cert.pem \
    -subj "/C=FR/ST=State/L=City/O=Org/OU=Dev/CN=localhost"

# Assure que le serveur peut lire les fichiers
RUN chmod 644 /certs/key.pem /certs/cert.pem

EXPOSE 5173

CMD ["npm", "run", "dev"]

