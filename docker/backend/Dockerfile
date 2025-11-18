#Build Stage
FROM node:20 AS builder

#Set working directory
WORKDIR /app

COPY package*.json ./

RUN npm install --omit=dev && npm cache clean --force

COPY /backend /app

#Production stage
FROM node:20 AS production

WORKDIR /app

#Create non-root user
RUN groupadd -g 1001 nodejs && \
	useradd -m -u 1001 -g nodejs nextjs

COPY --from=builder --chown=nextjs:nodejs /app /app

#Switch to non-root user
USER nextjs

#Expose port
EXPOSE 3000

EXPOSE 8000

# Add health check
HEALTHCHECK --interval=30s --timeout=3s --start-period=5s --retries=3 \
  CMD node -e "require('http').get('http://localhost:3000', (res) => { process.exit(res.statusCode === 200 ? 0 : 1) })"

#Starting Application
CMD ["node", "src/app.js"]

