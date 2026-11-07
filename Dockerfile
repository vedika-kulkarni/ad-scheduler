FROM node:20-bookworm

RUN apt-get update && apt-get install -y gcc g++ make && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY package*.json ./
RUN npm install

COPY . .

RUN gcc src/ad_scheduler.c -o src/ad_scheduler

ENV NODE_ENV=production

EXPOSE 10000

CMD ["npm", "start"]
