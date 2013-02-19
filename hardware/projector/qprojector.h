#ifndef QPROJECTOR_H
#define QPROJECTOR_H

#include "../qopticaldevice.h"
#include "../../gui/imageviewwidget.h"
#include "projectordependent.h"

/// Represents a projector.
class QProjector : public QOpticalDevice
{
    Q_OBJECT
public:
    /// Create a new projector.
    explicit QProjector(QObject *parent = 0);

    /// Represents the actual pattern being projected.
    class Pattern
    {
    public:
        /// Get the pattern.
        virtual QImage generatePattern(int width,
                                       int height)=0;
        /// Get an ID that uniquely identifies this pattern's
        /// configuration. This uniquely identifies the pattern
        /// within the queue, so it should be a determinate
        /// function of the pattern's class and parameters.
        virtual unsigned int getPatternID()=0;
    };

    /// Queue up a pattern.
    void queue(QProjector::Pattern *p);

    /// Register a ProjectorDependent, meaning
    /// the projector can't move on until all
    /// dependents have been satisfied.
    void registerDependency(ProjectorDependent *dp);

signals:
    /// Notify listeners that a pattern has been projected.
    void patternProjected(QProjector::Pattern*);

public slots:

    /// Check to see if all dependents are satisfied.
    bool checkDependencies();

    /// Deregister a dependent.
    void deregisterDependency(uint id);

    /// Deregister all dependents.
    void deregisterDependencies();

    /// Project the next pattern in the queue.
    /// \bug May emit patternProjected() before
    /// pattern is actually projected, depending
    /// on the implementation. Might want to move
    /// that to child processes.
    void processQueue();

protected:

    /// Project a pattern.
    virtual void projectPattern(QProjector::Pattern *){}

private:

    /// Queue of patterns waiting to be projected.
    std::vector<QProjector::Pattern*> patternQueue;

    /// List of dependents
    std::vector<ProjectorDependent*> dependents;
};

#endif // QPROJECTOR_H