#include "SpaceGameMode.h"

void ASpaceGameMode::BeginPlay()
{
	Super::BeginPlay();
	Lives = StartLives;
	Score = 0;
	bGameOver = false;
	OnStatsChanged.Broadcast();
}

void ASpaceGameMode::AddScore(int32 Points)
{
	if (bGameOver) return;
	Score += Points;
	OnStatsChanged.Broadcast();
}

void ASpaceGameMode::LoseLife()
{
	if (bGameOver) return;
	Lives = FMath::Max(0, Lives - 1);
	OnStatsChanged.Broadcast();

	if (Lives <= 0)
	{
		bGameOver = true;
		OnGameOver();
	}
}
