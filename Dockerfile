FROM node:8-slim

ENV APPDIR=/usr/src/app
ENV CRONDIR=/etc/cron.d

RUN apt-get update && apt-get -y install cron gcc make

WORKDIR ${APPDIR}
COPY app/package.json ${APPDIR}
ENV NODE_ENV=production
RUN npm install
COPY app ${APPDIR}

COPY crontab ${CRONDIR}/weather-cron
RUN chmod 0644 ${CRONDIR}/weather-cron
RUN crontab ${CRONDIR}/weather-cron

RUN npm run build

CMD ["cron", "-f"]
