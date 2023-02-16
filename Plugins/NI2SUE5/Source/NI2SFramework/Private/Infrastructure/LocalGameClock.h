// Copyright (c) 2023 ManOguaR's.

#pragma once

#include "Kismet/KismetMathLibrary.h"
#include "LocalGameClock.generated.h"

UENUM()
enum EGameSpeed
{
	EGameSpeed_TimeStopped			UMETA(DisplayName = "TimeStopped"),
	EGameSpeed_HourPerSecond		UMETA(DisplayName = "HourPerSecond"),
	EGameSpeed_DayPerMinute			UMETA(DisplayName = "DayPerMinute"),
	EGameSpeed_DayPerHour			UMETA(DisplayName = "DayPerHour"),
	EGameSpeed_Realtime				UMETA(DisplayName = "Realtime"),
};

/**
 *
 */
USTRUCT(BlueprintType)
struct FLocalGameClock
{
	GENERATED_BODY()

public:
	FORCEINLINE FLocalGameClock(EGameSpeed GameSpeed = EGameSpeed::EGameSpeed_HourPerSecond)
		: Seconds(0), Minutes(0), Hours(0), Day(1), Month(1), Year(1)
	{
		InitClock();
		SetGameSpeed(GameSpeed);
	}
	explicit FORCEINLINE FLocalGameClock(float GameSpeed)
		: Seconds(0), Minutes(0), Hours(0), Day(1), Month(1), Year(1)
	{
		InitClock();
		SetGameSpeed(GameSpeed);
	}
	explicit FORCEINLINE FLocalGameClock(int day, int month, int year, EGameSpeed GameSpeed = EGameSpeed::EGameSpeed_HourPerSecond)
		: Seconds(0), Minutes(0), Hours(0), Day(day), Month(month), Year(year)
	{
		InitClock();
		SetGameSpeed(GameSpeed);
	}
	explicit FORCEINLINE FLocalGameClock(int day, int month, int year, float GameSpeed)
		: Seconds(0), Minutes(0), Hours(0), Day(day), Month(month), Year(year)
	{
		InitClock();
		SetGameSpeed(GameSpeed);
	}
	explicit FORCEINLINE FLocalGameClock(int seconds, int minutes, int hours, int day, int month, int year, EGameSpeed GameSpeed = EGameSpeed::EGameSpeed_HourPerSecond)
		: Seconds(seconds), Minutes(minutes), Hours(hours), Day(day), Month(month), Year(year)
	{
		InitClock();
		SetGameSpeed(GameSpeed);
	}
	explicit FORCEINLINE FLocalGameClock(int seconds, int minutes, int hours, int day, int month, int year, float GameSpeed)
		: Seconds(seconds), Minutes(minutes), Hours(hours), Day(day), Month(month), Year(year)
	{
		InitClock();
		SetGameSpeed(GameSpeed);
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ClampMin = 0, ClampMax = 60))
	int Seconds;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ClampMin = 0, ClampMax = 60))
	int Minutes;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ClampMin = 0, ClampMax = 23))
	int Hours;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ClampMin = 1, ClampMax = 31))
	int Day;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ClampMin = 1, ClampMax = 12))
	int Month;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (ClampMin = 1))
	int Year;

	FORCEINLINE void UpdateFrame(float DeltaSeconds)
	{
		DayTick = UKismetMathLibrary::FMod((DeltaSeconds / TimeUnit) + Clockwork, 24.f, Clockwork);

		float remS, remM, remH;
		UKismetMathLibrary::FMod(UKismetMathLibrary::FMod(UKismetMathLibrary::FMod((Clockwork * 3600.f), 60.f, remS), 60.f, remM), 24.f, remH);

		Seconds = FMath::Floor(remS);
		Minutes = FMath::Floor(remM);
		Hours = FMath::Floor(remH);

		Day += DayTick;

		if (Day > UKismetMathLibrary::DaysInMonth(Year, Month))
		{
			Day = 1;
			Month++;
			if (Month > 12)
			{
				Month = 1;
				Year++;
			}
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red,    FString::Printf(TEXT("Delta:     %f"), DeltaSeconds));
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Orange, FString::Printf(TEXT("Clockwork: %f"), Clockwork));
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("Time:     %d:%d:%d"), Hours, Minutes, Seconds));
			GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, FString::Printf(TEXT("Calendar: %d/%d/%d"), Day, Month, Year));
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Orange, FString::Printf(TEXT("Clockwork: %d  [%d/%d/%d - %d:%d:%d]"), Clockwork, Day, Month, Year, Hours, Minutes, Seconds));
		}
	};

	FORCEINLINE void SetGameSpeed(EGameSpeed GameSpeed)
	{
		switch (GameSpeed)
		{
		case EGameSpeed_TimeStopped:
		{
			SetGameSpeed(0.f);
			break;
		}
		case EGameSpeed_Realtime:
		{
			SetGameSpeed(3600.f);
			break;
		}
		case EGameSpeed_DayPerHour:
		{
			SetGameSpeed(150.f);
			break;
		}
		case EGameSpeed_DayPerMinute:
		{
			SetGameSpeed(2.5f);
			break;
		}
		case EGameSpeed_HourPerSecond:
		{
			SetGameSpeed(1.f);
			break;
		}
		default:
			break;
		} 

	};

	FORCEINLINE void SetGameSpeed(float GameSpeed) { TimeUnit = GameSpeed; }

	FORCEINLINE float GetClockwork() { return Clockwork; }

private:
	float TimeUnit = 1.f; // 1.f => 1 second 1 hour, 2.5f => 1 minute 1 day (60/24), 150.f => 1 hour 1 day, 3600.f => realtime.
	float Clockwork;
	int DayTick;

	void InitClock()
	{
		Clockwork = ((float)Hours) + (((float)Minutes) / 60.f) + (((float)Seconds) / 3600.f);
	}
};
