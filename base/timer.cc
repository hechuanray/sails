// Copyright (C) 2014 sails Authors.
// All rights reserved.
//
// Filename: timer.cc
//
// Author: sailsxu <sailsxu@gmail.com>
// Created: 2014-10-11 10:47:09



#include "sails/base/timer.h"
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#ifdef __linux__
#ifdef __ANDROID__
#include <signal.h>
#else
#include <sys/timerfd.h>
#endif
#elif __APPLE__
#include <fcntl.h>
#endif


namespace sails {
namespace base {

#ifdef __APPLE__
int Timer::next_timerfd = 65535;
#endif

Timer::Timer(EventLoop *ev_loop, int tick) {
  this->tick = tick;
  this->ev_loop = ev_loop;
  this->data = NULL;
  self_evloop = 0;
  iothread = NULL;
}

Timer::Timer(int tick) {
  this->tick = tick;
  this->data = NULL;
  self_evloop = 0;
  ev_loop = NULL;
  iothread = NULL;
}

bool Timer::init(ExpiryAction action, void *data, int when = 1) {
#ifdef __linux__
  sails::base::EventLoop::Events events = sails::base::EventLoop::Event_READ;
  struct itimerspec new_value;
  new_value.it_interval.tv_sec = tick;
  new_value.it_interval.tv_nsec = 0;
  new_value.it_value.tv_sec = when;
  new_value.it_value.tv_nsec = 0;
#ifdef __ANDROID__
  struct sigevent sev;
  sev.sigev_notify = SIGEV_THREAD;
  sev.sigev_signo = SIGRTMIN;
  sev.sigev_notify_function = (ExpiryAction1)action;
  sev.sigev_notify_attributes = NULL;
  if (timer_create(CLOCK_REALTIME, &sev, &timerfd) == -1) {
    perror("timer_create");
    return false;
  }
  if (timer_settime(timerfd, 0, &new_value, NULL) == -1) {
    perror("timer_settime error");
    return false;
  }
  return true;
#else
  timerfd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);
  timerfd_settime(timerfd, 0, &new_value, NULL);
#endif
#elif __APPLE__
  // 在这儿的fd可以随便指定，但是不要和其它重复
  // 不要太小，否则可能会搞死人，以前指定的是1,
  // 结果导致由于关闭时会close,之后看不到输出，差点被搞崩溃
  //  timerfd = open("/tmp/temp_sails_event_poll.txt", O_CREAT | O_RDWR, 0644);
  timerfd = --next_timerfd;
  sails::base::EventLoop::Events events = sails::base::EventLoop::Event_TIMER;
#endif
  sails::base::event ev;
  emptyEvent(&ev);
  ev.fd = timerfd;
  ev.events = events;
  ev.cb = sails::base::Timer::read_timerfd_data;
#ifdef __APPLE__
  ev.edata = tick*1000;  // ms为单位
#endif
  ev.data.ptr = this;
  ev.next = NULL;

  if (ev_loop == NULL) {
    ev_loop = new EventLoop(this);
    ev_loop->init();
    self_evloop = 1;
    // 新建一个线程去运行 ev_loop->start_loop();
    iothread = new std::thread(std::bind(&EventLoop::start_loop, ev_loop));
  }

  if (!ev_loop->event_ctl(base::EventLoop::EVENT_CTL_ADD, &ev)) {
    close(timerfd);
    timerfd = 0;
    return false;
  }
  this->action = action;
  this->data = data;
  return true;
}

bool Timer::disarms()  {
  if (timerfd > 0) {
    ev_loop->event_stop(timerfd);
    close(timerfd);
    timerfd = 0;
  }
  return true;
}

void Timer::read_timerfd_data(base::event* ev, int revents, void* owner) {
  if (owner == NULL) {
    return;
  }
  Timer *timer = reinterpret_cast<Timer*>(ev->data.ptr);
  if (timer != NULL) {
#ifdef __linux__
    // linux的timer有数据要读
    memset(timer->temp_data, '\0', 50);
    int n = read(ev->fd, timer->temp_data, sizeof(uint64_t));
    if (n != sizeof(uint64_t)) {
      return;
    }
#endif
    timer->pertick_processing();
  }
}


Timer::~Timer() {
  if (timerfd > 0) {
    disarms();
  }
  if (self_evloop > 0) {
    ev_loop->stop_loop();  // iothread会停止
    iothread->join();
    delete iothread;
    iothread = NULL;
    delete ev_loop;
  }
  ev_loop = NULL;
  action = NULL;
}

void Timer::pertick_processing() {
  (*action)(this->data);
}



}  // namespace base
}  // namespace sails
