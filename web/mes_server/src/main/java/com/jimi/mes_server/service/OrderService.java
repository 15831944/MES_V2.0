package com.jimi.mes_server.service;

import com.jfinal.plugin.activerecord.Db;
import com.jfinal.plugin.activerecord.Record;
import com.jimi.mes_server.exception.ParameterException;
import com.jimi.mes_server.model.GpsManuorderparam;
import com.jimi.mes_server.service.base.SelectService;

/**
 * 订单业务层
 * <br>
 * <b>2018年5月29日</b>
 * @author 沫熊工作室 <a href="http://www.darhao.cc">www.darhao.cc</a>
 */
public class OrderService extends SelectService {


	public boolean update(GpsManuorderparam order) {
		GpsManuorderparam orderInDb = GpsManuorderparam.dao.findById(order.getId());
		if (orderInDb.getStatus() != 0) {
			throw new ParameterException("只能更新未开始的订单");
		}
		order.remove("Status");
		return order.update();
	}


	public boolean create(GpsManuorderparam order) {
		Record orderInDb = Db.findFirst("select * from Gps_ManuOrderParam where ZhiDan = ? and Status != 3", order.getZhiDan());
		if (orderInDb != null) {
			throw new ParameterException("订单号已存在");
		}
		order.setStatus(0);
		return order.save();
	}


	public boolean copy(String key) {
		GpsManuorderparam order = GpsManuorderparam.dao.findById(key);
		order.remove("Id", "SIMStart", "SIMEnd", "BATStart", "VIPStart", "VIPEnd", "BATEnd", "_MASK_FROM_V2");
		order.set("Status", 0).set("IMEIStart", "00000000000000").set("IMEIEnd", "00000000000000");
		return order.save();
	}


	public boolean start(String key) {
		GpsManuorderparam order = GpsManuorderparam.dao.findById(key);
		if (order.getStatus() == 0) {
			order.setStatus(1);
			return order.update();
		} else {
			throw new ParameterException("只能开始未开始的订单");
		}
	}


	public boolean finish(String key) {
		GpsManuorderparam order = GpsManuorderparam.dao.findById(key);
		if (order.getStatus() <= 1) {
			order.setStatus(2);
			return order.update();
		} else {
			throw new ParameterException("只能完成未开始和进行中的订单");
		}
	}


	public boolean cancel(String key) {
		GpsManuorderparam order = GpsManuorderparam.dao.findById(key);
		if (order.getStatus() <= 1) {
			order.setStatus(3);
			return order.update();
		} else {
			throw new ParameterException("只能作废未开始和进行中的订单");
		}
	}
}
