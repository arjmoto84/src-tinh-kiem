-- TimerHead.lua
-- By: Dan_Deng(2003-08-23) 计时器相关工具

FramePerSec = 18			-- 每秒帧数当作常量处理
CTime = 3600					-- 每个时辰按600秒（10分钟）计算

function GetRestSec()		-- 直接返回计时器剩余秒数
	return floor(GetRestTime() / FramePerSec)
end;

function GetRestCTime()			-- 获取计时器剩余时间，大于一个时辰时按中国时间转换
	x = floor(GetRestTime() / FramePerSec)
	if (x < CTime) then		-- 不足一个时辰
		y = x.." gi﹜"
	else
		y = floor(x / CTime).." gi� "
	end
	return y
end;

function GetTimerTask()			-- 通过计时器ID号解析相应任务
	
end