#pragma once
#include "pch.h"
#include <iostream>
#include "Windows.h"
#include <speechapi_cxx.h>

	using namespace std;
	using namespace Microsoft::CognitiveServices::Speech;

class SpeechController
{
public:
	SpeechController();
	~SpeechController();
	static string recognizeSpeech()
	{
		// Creates an instance of a speech config with specified subscription key and service region.
		// Replace with your own subscription key and service region (e.g., "westus").
		auto config = SpeechConfig::FromSubscription("6885cdb372d947e1be841cc96d3c9d3b", "westeurope");

		// Creates a speech recognizer.
		auto recognizer = SpeechRecognizer::FromConfig(config);
		cout << "Say something...\n";

		// Starts speech recognition, and returns after a single utterance is recognized. The end of a
		// single utterance is determined by listening for silence at the end or until a maximum of 15
		// seconds of audio is processed.  The task returns the recognition text as result. 
		// Note: Since RecognizeOnceAsync() returns only a single utterance, it is suitable only for single
		// shot recognition like command or query. 
		// For long-running multi-utterance recognition, use StartContinuousRecognitionAsync() instead.
		auto result = recognizer->RecognizeOnceAsync().get();

		// Checks result.
		if (result->Reason == ResultReason::RecognizedSpeech)
		{
			return result->Text;
		}
		else if (result->Reason == ResultReason::NoMatch)
		{
			cout << "NOMATCH: Speech could not be recognized." << std::endl;
		}
		else if (result->Reason == ResultReason::Canceled)
		{
			auto cancellation = CancellationDetails::FromResult(result);
			cout << "CANCELED: Reason=" << (int)cancellation->Reason << std::endl;

			if (cancellation->Reason == CancellationReason::Error)
			{
				cout << "CANCELED: ErrorCode= " << (int)cancellation->ErrorCode << std::endl;
				cout << "CANCELED: ErrorDetails=" << cancellation->ErrorDetails << std::endl;
				cout << "CANCELED: Did you update the subscription info?" << std::endl;
			}
		}
		return "Recognition failed";
	}

	static string wmain()
	{
		return recognizeSpeech();
	}
};

