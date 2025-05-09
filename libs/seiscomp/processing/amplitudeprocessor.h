/***************************************************************************
 * Copyright (C) gempa GmbH                                                *
 * All rights reserved.                                                    *
 * Contact: gempa GmbH (seiscomp-dev@gempa.de)                             *
 *                                                                         *
 * GNU Affero General Public License Usage                                 *
 * This file may be used under the terms of the GNU Affero                 *
 * Public License version 3.0 as published by the Free Software Foundation *
 * and appearing in the file LICENSE included in the packaging of this     *
 * file. Please review the following information to ensure the GNU Affero  *
 * Public License version 3.0 requirements will be met:                    *
 * https://www.gnu.org/licenses/agpl-3.0.html.                             *
 *                                                                         *
 * Other Usage                                                             *
 * Alternatively, this file may be used in accordance with the terms and   *
 * conditions contained in a signed written agreement between you and      *
 * gempa GmbH.                                                             *
 ***************************************************************************/


#ifndef SEISCOMP_PROCESSING_AMPLITUDEPROCESSOR_H
#define SEISCOMP_PROCESSING_AMPLITUDEPROCESSOR_H


#include <seiscomp/core/interfacefactory.h>
#include <seiscomp/processing/timewindowprocessor.h>
#include <seiscomp/math/filter/seismometers.h>
#include <seiscomp/datamodel/origin.h>
#include <seiscomp/datamodel/sensorlocation.h>
#include <seiscomp/datamodel/pick.h>
#include <seiscomp/client.h>

#include <functional>


namespace Seiscomp {

namespace Geo {

class GeoFeature;

}

namespace Processing {


DEFINE_SMARTPOINTER(AmplitudeProcessor);

class SC_SYSTEM_CLIENT_API AmplitudeProcessor : public TimeWindowProcessor {
	DECLARE_SC_CLASS(AmplitudeProcessor)

	// ----------------------------------------------------------------------
	//  Public types
	// ----------------------------------------------------------------------
	public:
		//! Amplitude calculation capabilities
		enum Capability {
			//! No supported capabilities
			NoCapability  = 0x0000,
			//! Supports different amplitude measure types
			MeasureType   = 0x0001,
			//! Supports different amplitude combiner if
			//! the amplitude is measured on different
			//! components
			Combiner      = 0x0002,
			CapQuantity
		};

		class SignalTime {
			public:
				SignalTime() = default;
				SignalTime(int v);
				SignalTime(double v);
				SignalTime(const char *); // throws invalid_argument
				SignalTime(const std::string &); // throws invalid_argument
				SignalTime(const SignalTime &time);

				/**
				 * @brief Sets the expression to a constant expression.
				 * @param v The new value for the signal time
				 * @return this
				 */
				SignalTime &operator=(double v);

				SignalTime &operator=(const SignalTime &time);

				/**
				 * @brief Adds an offset to the signal expression but not to
				 *        the compiled value. This only modifies the expression
				 *        by adding a constant offset.
				 * @param v The offset value for the signal time
				 * @return this
				 */
				SignalTime &operator+=(double v);

				/**
				 * @brief Substracts an offset to the signal expression but not
				 *        to the compiled value. This only modifies the
				 *        expression by substracting a constant offset.
				 * @param v The offset value for the signal time
				 * @return this
				 */
				SignalTime &operator-=(double v);

				operator double() const;

				/**
				 * @brief Sets and compiles a statement
				 * @param error Optional error message if compilation failed.
				 * @return Success flag
				 */
				bool set(const std::string &text, std::string *error = nullptr);

				std::string toString() const;
				bool isValid() const { return _exp != nullptr; }

				/**
				 * @brief Evaluates the expression.
				 * This method throws a StatusException if the evaluation fails.
				 * @param proc The source processor
				 * @param left If the time is the left time of a time window
				 */
				void evaluate(const AmplitudeProcessor *proc, bool left);

			private:
				Core::BaseObjectPtr _exp;
				OPT(double)         _value;
		};

		//! Configuration structure to store processing dependent
		//! settings
		struct Config {
			// The noise and signal time window expressions. Those
			// might depend on origin or travel time information
			// and are only evaluated during setEnvironment.
			SignalTime  noiseBegin{-35};
			SignalTime  noiseEnd{-5};
			SignalTime  signalBegin{-5};
			SignalTime  signalEnd{30};

			std::string ttInterface;
			std::string ttModel;

			double      snrMin{3}; /* default: 3 */
			double      minimumPeriod{-1}; /* default: -1 */
			double      maximumPeriod{-1}; /* default: -1 */

			double      minimumDistance{0}; /* default: 0 */
			double      maximumDistance{180}; /* default: 180 */
			double      minimumDepth{-1E6}; /* default: -1E6 */
			double      maximumDepth{1E6}; /* default: 1E6 */

			double      respTaper{5.0};
			double      respMinFreq{0.00833333};
			double      respMaxFreq{0};

			Math::SeismometerResponse::WoodAnderson::Config woodAndersonResponse;

			// If true, compute amplitudes according to the recommendations of the
			// IASPEI CoSOI Magnitude Working Group. Currently only affects mb.
			bool       iaspeiAmplitudes{false};
		};

		struct Locale : Config {
			enum Check {
				Source,
				SourceReceiver,
				SourceReceiverPath
			};

			std::string            name;
			const Geo::GeoFeature *feature;
			Check                  check;
			Core::BaseObjectPtr    extra;
		};

		struct Environment {
			Environment();

			std::string                     networkCode;
			std::string                     stationCode;
			std::string                     locationCode;
			std::string                     channelCode;

			const DataModel::Origin         *hypocenter{nullptr};
			const DataModel::SensorLocation *receiver{nullptr};
			const DataModel::Pick           *pick{nullptr};

			const Locale                    *locale{nullptr};
		};

		struct AmplitudeIndex {
			double  index;
			double  begin;
			double  end;
		};

		struct AmplitudeTime {
			AmplitudeTime() : begin(0), end(0) {}
			AmplitudeTime(const Core::Time &ref)
			: reference(ref), begin(0), end(0) {}
			Core::Time  reference;
			double      begin;
			double      end;
		};

		struct AmplitudeValue {
			double      value;
			OPT(double) lowerUncertainty;
			OPT(double) upperUncertainty;
		};

		struct Result {
			StreamComponent component;
			const Record   *record;
			AmplitudeValue  amplitude;
			AmplitudeTime   time;
			double          period;
			double          snr;
		};

		using IDList = std::vector<std::string>;

		using PublishFunc = std::function<void (const AmplitudeProcessor*,
		                                        const Result &)>;


	// ----------------------------------------------------------------------
	//  X'truction
	// ----------------------------------------------------------------------
	public:
		//! C'tor
		AmplitudeProcessor();
		AmplitudeProcessor(const std::string &type);

		//! D'tor
		~AmplitudeProcessor();


	// ----------------------------------------------------------------------
	//  Configuration Interface
	// ----------------------------------------------------------------------
	public:
		//! Set the start of the noise window relative to the trigger
		void setNoiseStart(const SignalTime &start) { _config.noiseBegin = start; }

		//! Set the end of the noise window relative to the trigger
		void setNoiseEnd(const SignalTime &end)  { _config.noiseEnd = end; }

		//! Set the start of the signal window relative to the trigger
		void setSignalStart(const SignalTime &start)  { _config.signalBegin = start; }

		//! Set the end of the signal window relative to the trigger
		void setSignalEnd(const SignalTime &end)  { _config.signalEnd = end; }

		void setMinSNR(double snr) { _config.snrMin = snr; }

		//! Sets the minimum measured period required
		void setMinPeriod(double period) { _config.minimumPeriod = period; }

		//! Sets the maximum measured period required
		void setMaxPeriod(double period) { _config.maximumPeriod = period; }

		//! Sets the minimum distance to calculate amplitudes for
		void setMinDist(double dist) { _config.minimumDistance = dist; }

		//! Sets the maximum distance to calculate amplitudes for
		void setMaxDist(double dist) { _config.maximumDistance = dist; }

		//! Sets the minimum depth to calculate amplitudes for
		void setMinDepth(double depth) { _config.minimumDepth = depth; }

		//! Sets the maximum depth to calculate amplitudes for
		void setMaxDepth(double depth) { _config.maximumDepth = depth; }

		//! Sets a configuration
		void setConfig(const Config &config) { _config = config; }

		//! Returns the current configuration
		const Config &config() const { return _config; }

		/**
		 * @brief Sets the environment for the amplitude processor. Basically
		 *        it is the hypocenter, the receiver and the pick made. The
		 *        pick time must correspond to the trigger time set.
		 *        This method was added with API 12.
		 * @param hypocenter The hypocenter or null
		 * @param receiver The receiver or null
		 * @param pick The pick or null
		 */
		virtual void setEnvironment(const DataModel::Origin *hypocenter,
		                            const DataModel::SensorLocation *receiver,
		                            const DataModel::Pick *pick);

		const Environment &environment() const { return _environment; }

		//! Sets whether amplitude updates are enabled or not
		void setUpdateEnabled(bool);
		bool isUpdateEnabled() const;

		void setReferencingPickID(const std::string&);
		const std::string& referencingPickID() const;

		void setPick(const DataModel::Pick *pick);
		const DataModel::Pick *pick() const;

		static bool CreateAlias(const std::string &aliasType,
		                        const std::string &sourceType);
		static bool RemoveAlias(const std::string &aliasType);
		static void RemoveAllAliases();


	// ----------------------------------------------------------------------
	//  Query interface
	//  This interface is important to be implemented for interactive
	//  analysis.
	// ----------------------------------------------------------------------
	public:
		//! Returns a child processor for a specific component. The
		//! returned pointer must not be deleted. The default
		//! implementation returns 'this' if comp matched the usedComponent.
		virtual const AmplitudeProcessor *componentProcessor(Component comp) const;

		//! Returns the internally processed data which is used to
		//! measure the amplitude.
		//! The default implementation returns
		//! TimeWindowProcessor::continuousData.
		//! The returned pointer must not be managed by a smartpointer or
		//! deleted. It points to a static member of this class.
		virtual const DoubleArray *processedData(Component comp) const;

		//! Returns the implementations capabilities
		//! The default implementation returns NoCapability (0)
		virtual int capabilities() const;

		//! Queries for a capability.
		bool supports(Capability capability) const;

		//! Returns a list of tokens valid for a certain capability
		//! The default implementation returns an empty list
		virtual IDList capabilityParameters(Capability cap) const;

		//! Sets a processing parameter. Value must be part of the
		//! list returned by capabilityParameters(cap).
		//! The default implementation returns always false.
		virtual bool setParameter(Capability cap, const std::string &value);

		//! Returns the configured value associated with a given capability.
		//! The default implementation returns an empty string.
		virtual std::string parameter(Capability cap) const;


	// ----------------------------------------------------------------------
	//  Public Interface
	// ----------------------------------------------------------------------
	public:
		virtual bool initLocale(Locale *locale, const Settings &settings);

		//! Reprocesses the current data chunk and searches for amplitudes
		//! only in the given optional time window relative to trigger time
		//! (if supported by the implementation).
		virtual void reprocess(OPT(double) searchBegin = Core::None,
		                       OPT(double) searchEnd = Core::None);

		//! Resets the amplitude processor and deletes all data
		//! and noise amplitudes
		virtual void reset() override;

		//! This method has to be called when all configuration
		//! settings have been set to calculate the timewindow
		virtual void computeTimeWindow() override;

		//! Sets up the amplitude processor. By default it reads whether
		//! to use response information or not.
		virtual bool setup(const Settings &settings) override;

		//! Sets the trigger used to compute the timewindow to calculate
		//! the amplitude
		//! Once a trigger has been set all succeeding calls will fail.
		virtual void setTrigger(const Core::Time &trigger);

		Core::Time trigger() const;

		/**
		 * @brief Allows to finalize an amplitude object as created by
		 *        client code.
		 *
		 * This method will usually be called right before the amplitude will
		 * be stored or sent and inside the emit handler. It allows processors
		 * to set specific attributes or to add comments.
		 * The default implementation does nothing.
		 * @param amplitude The amplitude to be finalized
		 */
		virtual void finalizeAmplitude(DataModel::Amplitude *amplitude) const;

		void setPublishFunction(const PublishFunc &func);

		//! Returns the computed noise offset
		OPT(double) noiseOffset() const;

		//! Returns the computed noise amplitude
		OPT(double) noiseAmplitude() const;

		//! Returns the type of amplitude to be calculated
		const std::string &type() const;

		//! Returns the unit of amplitude to be calculated
		const std::string& unit() const;

		//! Dumps the record data into an ascii file
		void writeData() const;


	// ----------------------------------------------------------------------
	//  Protected Interface
	// ----------------------------------------------------------------------
	protected:
		//! Sets the unit of the computed amplitude.
		void setUnit(const std::string &unit);

		virtual void process(const Record *record);

		void initFilter(double fsamp) override;

		bool handleGap(Filter *filter, const Core::TimeSpan&,
		               double lastSample, double nextSample,
		               size_t missingSamples) override;

		//! Method to prepare the available data just before the noise
		//! and amplitude calculation takes place. This method can be
		//! reimplemented to convert the data into velocity if the input
		//! signal unit is e.g. meter per second squared.
		virtual void prepareData(DoubleArray &data);

		//! Deconvolve the data using the sensor response. The default
		//! implementation simply calls:
		//! resp->deconvolveFFT(data, _fsamp, 60.0, 0.00833333, 0, numberOfIntegrations)
		//! which corresponds to a period lowpass (freq highpass) filter
		//! of 120 seconds.
		virtual bool deconvolveData(Response *resp, DoubleArray &data,
		                            int numberOfIntegrations);

		//! -----------------------------------------------------------------------
		//! Computes the amplitude of data in the range[i1, i2].
		//! -----------------------------------------------------------------------
		//! Input:
		//! -----------------------------------------------------------------------
		//!  - data: the waveform data
		//!  - offset: the computed noise offset
		//!  - i1: start index in data (trigger + config.signalBegin)
		//!  - i2: end index in data (trigger + config.signalEnd)
		//!  - si1: start index of the amplitude search window
		//!  - si2: end index of the amplitude search window
		//!  NOTE: si1 and si2 are guaranteed to be in range [i1,i2] if
		//!  the default AmplitudeProcessor::process method is called (especially
		//!  when reimplemented).
		//! -----------------------------------------------------------------------
		//! Output:
		//! -----------------------------------------------------------------------
		//!  - dt: the picked data index (can be a subindex if required)
		//!        the dt.begin and dt.end are the begin/end of the timewindow
		//!        in samples relativ to the picked index. dt.begin and dt.end
		//!        do not need to be in order, they are ordered afterwards
		//!        automatically. The default values for begin/end are 0.
		//!  - amplitude: the picked amplitude value with optional uncertainties.
		//!  - period: the period in samples and not seconds (-1 if not calculated)
		//!  - snr: signal-to-noise ratio
		//! -----------------------------------------------------------------------
		virtual bool computeAmplitude(const DoubleArray &data,
		                              size_t i1, size_t i2,
		                              size_t si1, size_t si2,
		                              double offset,
		                              AmplitudeIndex *dt,
		                              AmplitudeValue *amplitude,
		                              double *period, double *snr) = 0;

		//! Computes the noise of data in the range [i1,i2] and returns the offset and
		//! the amplitude in 'offset' and 'amplitude'
		//! The default implementation takes the median of the data as offset and
		//! twice the rms regarding the offset as amplitude
		virtual bool computeNoise(const DoubleArray &data, int i1, int i2, double *offset, double *amplitude);

		//! This method gets called when an amplitude has to be published
		void emitAmplitude(const Result &result);


	private:
		bool readLocale(Locale *locale,
		                const Settings &settings,
		                const std::string &configPrefix);
		void checkEnvironmentalLimits();


	private:
		void init();
		bool initRegionalization(const Settings &settings);
		void process(const Record *record, const DoubleArray &filteredData) override;


	// ----------------------------------------------------------------------
	//  Protected Members
	// ----------------------------------------------------------------------
	protected:
		OPT(Core::Time) _trigger;

		// User defined amplitude search window
		OPT(double)     _searchBegin, _searchEnd;

		// pre-arrival offset and rms
		OPT(double)     _noiseOffset, _noiseAmplitude, _lastAmplitude;

		bool            _enableUpdates;
		bool            _enableResponses;

		// config
		Config          _config;
		Environment     _environment;

		std::string     _type;
		std::string     _unit;

		std::string     _pickID;

		bool            _responseApplied;


	// ----------------------------------------------------------------------
	//  Private Members
	// ----------------------------------------------------------------------
	private:
		PublishFunc _func;


	friend class AmplitudeProcessorAliasFactory;
};


inline AmplitudeProcessor::Environment::Environment()
: hypocenter(nullptr), receiver(nullptr), pick(nullptr) {}

inline const DataModel::Pick *AmplitudeProcessor::pick() const {
	return _environment.pick;
}


DEFINE_INTERFACE_FACTORY(AmplitudeProcessor);


}
}


#define REGISTER_AMPLITUDEPROCESSOR_VAR(Class, Service) \
Seiscomp::Core::Generic::InterfaceFactory<Seiscomp::Processing::AmplitudeProcessor, Class> __##Class##InterfaceFactory__(Service)

#define REGISTER_AMPLITUDEPROCESSOR(Class, Service) \
static REGISTER_AMPLITUDEPROCESSOR_VAR(Class, Service)


#endif
