/*
 * Copyright (c) 2022. Alibaba Group Holding Limited
 */

package com.alibaba.hologres.client.impl.binlog;

import com.alibaba.hologres.client.utils.Tuple;

import java.util.concurrent.BlockingQueue;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

/**
 * 用来提交lsn的.
 */
public class Committer {
	final BlockingQueue<Tuple<CompletableFuture<Void>, Long>> queue;

	long lastReadLsn = -1;

	public Committer(BlockingQueue<Tuple<CompletableFuture<Void>, Long>> queue) {
		this.queue = queue;
	}

	public void updateLastReadLsn(long lastReadLsn) {
		this.lastReadLsn = lastReadLsn;
	}

	public CompletableFuture<Void> commit(long timeout) throws InterruptedException, TimeoutException {
		return commit(lastReadLsn, timeout);
	}

	public CompletableFuture<Void> commit(long lsn, long timeout) throws InterruptedException, TimeoutException {
		CompletableFuture<Void> future = new CompletableFuture<>();
		boolean ret = queue.offer(new Tuple<>(future, lsn), timeout, TimeUnit.MILLISECONDS);
		if (!ret) {
			throw new TimeoutException();
		} else {
			return future;
		}
	}

	public long getLastReadLsn() {
		return lastReadLsn;
	}
}
