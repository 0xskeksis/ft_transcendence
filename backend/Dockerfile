# =========================
# Stage: builder
# =========================
FROM node:20 AS builder
WORKDIR /app

COPY ./backend/package*.json ./
RUN npm install --omit=dev && npm cache clean --force

COPY backend /app

# Build native addon
WORKDIR /app/src/pong
RUN npx node-gyp configure && npx node-gyp build

# =========================
# Stage: production
# =========================
FROM node:20 AS production
WORKDIR /app

# create user
RUN groupadd -g 1001 nodejs && \
    useradd -m -u 1001 -g nodejs nextjs

# copy app files
COPY --from=builder /app /app
RUN mkdir /certs
RUN chmod 777 /certs

# copy addon explicitly and fix permissions
COPY --from=builder --chown=nextjs:nodejs \
    /app/src/pong/build/Release/addon.node \
    /app/src/pong/build/Release/addon.node

RUN echo "FR\n\r.\n\r.\n\r.\n\r.\n\r.\n\r" | openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout /certs/key.pem -out /certs/cert.pem

RUN chmod 777 /certs/key.pem
RUN chmod 777 /certs/cert.pem

USER nextjs

EXPOSE 3000 8000


CMD ["npm", "run", "dev"]
